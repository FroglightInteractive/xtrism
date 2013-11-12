#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

main() {
  int fd=open("test.out.gz",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
  int pfd[2];
  pipe(pfd);
  printf("fd=%i pfd[0]=%i pfd[1]=%i\n",fd,pfd[0],pfd[1]);
  int f=fork();
  printf("fork returned %i\n",f);
  if (f==0)
    { printf("childproc\n");
      close(pfd[1]);
      close(0); dup(pfd[0]);
      close(1); dup(fd);
      execlp("gzip","gzip","-",0);
      fprintf(stderr,"execlp returned\n");
      exit(1);
    }

  printf("parentproc\n");
  close(pfd[0]); close(fd);
  char msg[]="Hello world\nThis is a test\n\0";
  int l=strlen(msg);
  printf("writing %i-%i\n",l,write(pfd[1], msg,l));
  close(pfd[1]);
//  kill(f,1);
  int stt;
  printf("waiting\n");
  wait(&stt);
  printf("stt=%i\n",stt);
  return 0;
  }

  
