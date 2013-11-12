// Sounds.H - the collection of game sounds

#ifndef _Sounds_H
#define _Sounds_H

class Sounds {
  public:
    Sounds(class Filename const &dir) throw (char const *);
    ~Sounds();
    void speedup(float posn=0) const;
    void turn(float posn=0) const;
    void drop(float posn=0) const;
    void applause(float posn=0) const;
    void shoosh(float posn=0) const;
    void explode(float posn=0, float ampl=1) const;
    void warn(float posn=0) const;
    bool poll();
    void activate();
    void deactivate();
    class SPlayer *player() { return splayer; } // only used by Globals.C!
  private:
    class SPlayer *splayer;
    class Sample *speedup_, *turn_, *drop_, *applause_;
    class Sample *shoosh_, *explode_, *warn_;
  };

extern class Sounds *sounds;

#endif
