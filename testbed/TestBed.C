// TestBed.C

#include "TestBed.H"

TestBed::TestBed(TEnv const &env):
  tenv(env),
  rgbmap(env.width(), env.height()),
  timage(env,env.width(), env.height()),
  tsprite(env,env.width(), env.height()) {
  }
