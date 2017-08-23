// KbdBuffer.C

// Waker info: sends warn() when the buffer state changes from empty
// to non-empty (only).

#include <string>

#include "KbdBuffer.H"
#include "../basics/Throw.H"

/////////////////////////////////////////////////////////////////////////////
KeyCodes::KeyCodes(TKeyCode const *kcs) {
  for (int i = 0; i < KeyNumber(KN_Max); i++)
    data[i] = kcs[i];
}

KeyCodes::KeyCodes(vector<TKeyCode> const &kcs) {
  for (int i = 0; i < KeyNumber(KN_Max); i++)
    data[i] = kcs[i];
}

KeyCodes::KeyCodes(TKeyCode const *user, TKeyCode const *global) {
  setuser(user);
  setglobal(global);
}

KeyCodes::KeyCodes(vector<TKeyCode> const &user,
                   vector<TKeyCode> const &global) {
  setuser(user);
  setglobal(global);
}

KeyCodes::KeyCodes(TKeyCode const *user,
                   vector<TKeyCode> const &global) {
  setuser(user);
  setglobal(global);
}

void KeyCodes::setuser(TKeyCode const *user) {
  for (int i = KN_First; i < KN_MaxUser; i++)
    data[i] = user ? user[i] : -1;
}

void KeyCodes::setuser(vector<TKeyCode> const &user) {
  for (int i = KN_First; i < KN_MaxUser; i++)
    data[i] = user[i];
}

void KeyCodes::setglobal(TKeyCode const *global) {
  for (int i = KN_FirstGlobal; i < KN_Max; i++)
    data[i] = global[i - KN_FirstGlobal];
}

void KeyCodes::setglobal(vector<TKeyCode> const &global) {
  for (int i = KN_FirstGlobal; i < KN_Max; i++)
    data[i] = global[i - KN_FirstGlobal];
}

/////////////////////////////////////////////////////////////////////////////
bool KbdBuffer::enter(TKeyCode kc, bool in_not_out) {
  KeyNumber n(KN_None);
  int i;
  for (i = KN_First; i < KN_Max; i++)
    if (kc == kcs[KeyNumber(i)])
      n = KeyNumber(i);
  if (n == KN_None)
    return false;

  if (in_not_out) {
    state = BufferState(state | (1 << n));
    switch (n) {
    case KN_Left:
      if ((state & BS_Right) && (state & BS_Zap))
        ithreekey(KN_Left);
      else {
        if (state & BS_Zap)
          enter(BC_iZLeft);
        else
          enter(BC_iLeft);
      }
      last = KN_Left;
      break;

    case KN_Right:
      if ((state & BS_Left) && (state & BS_Zap))
        ithreekey(KN_Right);
      else {
        if (state & BS_Zap)
          enter(BC_iZRight);
        else
          enter(BC_iRight);
      }
      last = KN_Right;
      break;

    case KN_RotCW:
      enter(BC_iRotCW);
      break;

    case KN_RotCCW:
      enter(BC_iRotCCW);
      break;

    case KN_Drop:
      enter(BC_iDrop);
      break;

    case KN_Zap:
      enter(BC_iZap);
      if ((state & BS_Left) && (state & BS_Right))
        ithreekey(KN_Zap);
      else {
        if (state & BS_Left)
          enter(BC_iZLeft);
        if (state & BS_Right)
          enter(BC_iZRight);
      }
      break;

    case KN_Pause:
      enter(BC_iPause);
      break;

    case KN_LevUp:
      enter(BC_iLevUp);
      break;

    case KN_LevDn:
      enter(BC_iLevDn);
      break;

    case KN_Quit:
      enter(BC_iQuit);
      break;

    case KN_None:
    case KN_Max:
      break;                              // shouldn't happen
    }
  } else {
    state = BufferState(state & ~(1 << n));
    switch (n) {
    case KN_Left:
      if ((state & BS_Right) && (state & BS_Zap))
        othreekey(KN_Left);
      else
        enter(BC_oLeft);
      break;

    case KN_Right:
      if ((state & BS_Left) && (state & BS_Zap))
        othreekey(KN_Right);
      else
        enter(BC_oRight);
      break;

    case KN_Zap:
      if ((state & BS_Left) && (state & BS_Right))
        othreekey(KN_Zap);
      else {
        enter(BC_oZap);
        if (state & BS_Left)
          enter(BC_oZLeft);
        else if (state & BS_Right)
          enter(BC_oZRight);
      }
      break;

    case KN_Drop:
    case KN_RotCW:
    case KN_RotCCW:
      break;

    case KN_Pause:
    case KN_Quit:
      break;

    case KN_LevUp:
      enter(BC_oLevUp);
      break;

    case KN_LevDn:
      enter(BC_oLevDn);
      break;

    case KN_None:
    case KN_Max:
      break;                              // shouldn't happen
    }
  }
  return true;
}

void KbdBuffer::enter(BufferCode b) {
  if (data.empty())
    sendwarn();
  data.push_back(b);
/*  data[wp]=b;
   if (wp.inc() == rp)
    athrow("KbdBuffer: overflow");*/
}

void KbdBuffer::putback(BufferCode b) {
  if (data.empty())
    sendwarn();
  data.push_front(b);
}

BufferCode KbdBuffer::read() {
  if (data.empty())
    return BufferCode(BC_None);
  else {
    BufferCode b = *data.begin();
    data.erase(data.begin());
    return b;
  }
  /* if (wp == rp)
     return BufferCode(BC_None);
     BufferCode r(data[rp]);
     rp.inc();
     return r; */
}

void KbdBuffer::ithreekey(KeyNumber nw) {
  switch (nw) {
  case KN_Left:
    enter(BC_oZRight);
    break;

  case KN_Right:
    enter(BC_oZLeft);
    break;

  case KN_Zap:
    if (last == KN_Left) {
      enter(BC_iZLeft);
      enter(BC_oZLeft);
    } else if (last == KN_Right) {
      enter(BC_iZRight);
      enter(BC_oZRight);
    }
    break;

  default:
    break;
  }
}

void KbdBuffer::othreekey(KeyNumber nw) {
  switch (nw) {
  case KN_Left:
    enter(BC_oLeft);
    enter(BC_iZRight);
    break;

  case KN_Right:
    enter(BC_oRight);
    enter(BC_iZLeft);
    break;

  case KN_Zap:
    enter(last == KN_Right ? BC_iRight : BC_iLeft);
    break;

  default:
    break;
  }
}