// SPlayer.C

#include "SPlayer.H"
#include "Sample.H"

#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include "../basics/minmax.H"
#include "../basics/dbx.H"

SPlayer::SPlayer(): fd(-1), dummy(false) {
  buf.chr=0;
  }

void SPlayer::activate() throw (SampleExc) {
  if (dummy) return;
  if (fd>=0)
    { if (deact)
        donedeact();
      else return;
    }
  dummy=true;
  deact=0;
  
  fd=open(SP_SOUNDDEV,O_WRONLY);
  if (fd<0)
    { perror("SPlayer: " SP_SOUNDDEV); throw SampleExc(); }

  int mask;
  int bytespsam;
  // ioctl(fd, SNDCTL_DSP_RESET, 0);
  
  if (ioctl(fd,SNDCTL_DSP_GETFMTS,&mask)<0)
    { perror("SPlayer: SNDCTL_DSP_GETFMTS"); throw SampleExc(); }
  if (mask & AFMT_S16_LE)
    { int fmt=AFMT_S16_LE;
      if (ioctl(fd,SNDCTL_DSP_SETFMT,&fmt)<0)
        { perror("SPlayer: SNDCTL_DSP_SETFMT"); throw SampleExc(); }
      p=&SPlayer::polls16; bytespsam=2;
    }
  else if (mask & AFMT_U8)
    { int fmt=AFMT_U8;
      if (ioctl(fd,SNDCTL_DSP_SETFMT,&fmt)<0)
        { perror("SPlayer: SNDCTL_DSP_SETFMT"); throw SampleExc(); }
      p=&SPlayer::pollu8; bytespsam=1;
    }
  else
    { fprintf(stderr,"SPlayer: can only do S16LE or U8 formats\n");
      throw SampleExc();
    }

  int s=1;
  if (ioctl(fd,SNDCTL_DSP_STEREO,&s)<0)
    { perror("SPlayer: SNDCTL_DSP_STEREO"); throw SampleExc(); }
  stereo=(s==1);

  int spd=SP_DFLTSPD;
  if (ioctl(fd,SNDCTL_DSP_SPEED,&spd)<0)
    { perror("SPlayer: SNDCTL_DSP_SPEED"); throw SampleExc(); }
  pfreq=spd;

  int bytespsec=(1+stereo)*bytespsam*pfreq; // bytes per second
  int optifragsize=bytespsec/SP_FRAGSPSEC; // optimal fragment size
  int logfrag=max(int(floor(log(double(optifragsize))/log(2.)+.5)), 8);
  int fragsize=1<<logfrag; // nearest realizable fragment size
  int arg=logfrag+(10000<<16);
  if (ioctl(fd,SNDCTL_DSP_SETFRAGMENT,&arg)<0)
    { perror("SPlayer: SNDCTL_DSP_SETFRAGMENT"); throw SampleExc(); }
  audio_buf_info info;
  if (ioctl(fd,SNDCTL_DSP_GETOSPACE, &info)<0)
    { perror("SPlayer: SNDCTL_DSP_GETOSPACE"); throw SampleExc(); }
  int nfrags
    = max(int((bytespsec*SP_MAXAHEAD)/fragsize), 2); // # frags I want to use  
  freefrags=info.fragstotal-nfrags; // leave rest free
  dbx(-980808,"SPlayer: nfrags=%i, info.fragstotal=%i, info.fragments=%i",nfrags,info.fragstotal,info.fragments);
  dbx(-980808,"SPlayer: fragsize=%i, info.fragsize=%i info.bytes=%i",fragsize,info.fragsize,info.bytes);

  /* ugly buffering stuff follows. should be hidden somehow. */
  buf.chr=new char[bufsize=fragsize];
  fragsams=fragsize/(bytespsam*(1+stereo));
  
  n=0; lastsph=0;
  dummy=false;
  }

SPlayer::~SPlayer() {
  if (fd>=0)
    close(fd);
  if (buf.chr)
    delete [] buf.chr;
  }

void SPlayer::deactivate() {
  if (fd<0)
    return; // already inactive
  deact=true;
//  freefrags=0;
  poll();
  if (n==0)
    donedeact();
  }

void SPlayer::donedeact() {
  close(fd);
  fd=-1;
  if (buf.chr)
    delete [] buf.chr;
  buf.chr=0;
  }

SPHandle SPlayer::play(Sample const *s, float freqratio,
                       float amplitude, float posn) throw(SampleExc) {
  if (dummy) return -1;
  activate();
//  printf("play: stereo=%i pfreq=%ui fragsams=%ui n=%i bufsize=%i\n",
//	   int(stereo),pfreq,fragsams,n,bufsize);
//  printf("\tsample=%p, freqratio=%g, ampl=%g, posn=%g\n",
//	   s,freqratio,amplitude,posn);
//  printf("\tsfreq=%i, length=%i\n",s->sfreq(), s->length()); 
  
  if (n==MAXSAMPLES)
    { fprintf(stderr,"SPlayer: Max # of samples (%i) reached\n",MAXSAMPLES);
      return -1;
    }
  sounds[n].sam=s;
  sounds[n].ampl[0]=amplitude*(stereo?(1-posn)/2:1);
  sounds[n].ampl[1]=amplitude*(stereo?(1+posn)/2:1);
  sounds[n].rate=freqratio*s->sfreq()/pfreq;
  sounds[n].length=s->length();
  sounds[n].offset=0;
  sounds[n].sph=lastsph;
  n++;
  return lastsph++;
  }

void SPlayer::stop(SPHandle const &sph) {
  for (int i=0; i<n; i++)
    if (sounds[i].sph==sph)
      { dostop(i);
        if (!n)
          quiet();
        return;
      }
  }

void SPlayer::stop() {
  while (n)
    dostop(n-1);
  quiet();
  }

void SPlayer::dostop(int k) {
  for (int l=k+1; l<n; l++)
    sounds[l-1]=sounds[l];
  n--;
  }

bool SPlayer::poll() throw (SampleExc) {
  if (dummy || fd<0)
    return 0; // not active
  audio_buf_info info;
  if (ioctl(fd,SNDCTL_DSP_GETOSPACE,&info)<0)
    { perror("SPlayer: SNDCTL_DSP_GETOSPACE"); throw SampleExc(); }
  //  printf("SP:poll: f=%i ft=%i fs=%i b=%i (ff=%i)\n",
  //          info.fragments, info.fragstotal, info.fragsize, info.bytes, freefrags);
//  freefrags=0;
  if (info.fragments>int(freefrags))
    { if (deact)
        for (int i=0; i<int(info.fragments-freefrags); i++)
          { (this->*p)(1);
            if (n==0)
              { donedeact(); break; }
          }
        else
          (this->*p)(info.fragments-freefrags);
      return 1;
    }
  return 0;
  }

void SPlayer::quiet() const {
  // ioctl(fd, SNDCTL_DSP_POST, 0);
  }

void SPlayer::polls16(int nfrags) throw (SampleExc) {
//  printf("buf=%p=%p bufsize=%i\n",buf.s16,buf.chr,bufsize);
//  printf("fragsams=%i\n",fragsams);
//  if (!n)
//    return;
  for (int i=0; i<nfrags; i++)
    { signed short *bufptr=buf.s16;
      if (stereo)
        for (unsigned int j=0; j<fragsams; j++)
          { float left=0, right=0;
            for (int k=0; k<n; k++)
              { float c=(*sounds[k].sam)[int(sounds[k].offset)];
                left+=c*sounds[k].ampl[0];
                right+=c*sounds[k].ampl[1];
                sounds[k].offset+=sounds[k].rate;
                if (sounds[k].offset >= sounds[k].length)
                  dostop(k);
              }
            *(bufptr++)=short(left);
            *(bufptr++)=short(right);
          }
      else // mono
        for (unsigned int j=0; j<fragsams; j++)
          { float mono=0;
            for (int k=0; k<n; k++)
              { float c=(*sounds[k].sam)[int(sounds[k].offset)];
                mono+=c*sounds[k].ampl[0];
                sounds[k].offset+=sounds[k].rate;
                if (sounds[k].offset >= sounds[k].length)
                  dostop(k);
              }
            *(bufptr++)=short(mono);
          }
      if (write(fd,buf.chr,bufsize)<0)
        { perror("SPlayer: write()"); throw SampleExc(); }
    }
  if (n==0)
    quiet();
  }

void SPlayer::pollu8(int /*nfrags*/) throw (SampleExc) {
  perror("SPlayer: pollu8 NYI");
  throw SampleExc();
  }
