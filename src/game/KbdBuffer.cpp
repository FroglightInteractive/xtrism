// KbdBuffer.C

// Waker info: sends warn() when the buffer state changes from empty
// to non-empty (only).

#include <string>
#include "MapIterator.h"
#include "KbdBuffer.h"

KeyCodes::KeyCodes(GameKeys const &gk, MetaKeys const &mk):
  gk(gk), mk(mk) {
  rebuild();
}

void KeyCodes::setuser(GameKeys const &gk1) {
  gk = gk1;
  rebuild();
}

void KeyCodes::setglobal(MetaKeys const &mk1) {
  mk = mk1;
  rebuild();
}

void KeyCodes::rebuild() {
  clear();
  (*this)[gk[GameKeys::Key::Left]] = KN_Left;
  (*this)[gk[GameKeys::Key::Right]] = KN_Right;
  (*this)[gk[GameKeys::Key::CW]] = KN_RotCW;
  (*this)[gk[GameKeys::Key::CCW]] = KN_RotCCW;
  (*this)[gk[GameKeys::Key::Drop]] = KN_Drop;
  (*this)[gk[GameKeys::Key::Zap]] = KN_Zap;
  (*this)[gk[GameKeys::Key::FloatDown]] = KN_FloatDown;

  (*this)[mk[MetaKeys::Key::Pause]] = KN_Pause;
  (*this)[mk[MetaKeys::Key::LevDown]] = KN_LevDn;
  (*this)[mk[MetaKeys::Key::LevUp]] = KN_LevUp;
  (*this)[mk[MetaKeys::Key::Quit]] = KN_Quit;
}


/////////////////////////////////////////////////////////////////////////////
KbdBuffer::KbdBuffer(GameKeys const &gk, MetaKeys const &mk):
    kcs(gk, mk),  // rp(KB_BUFSIZE), wp(KB_BUFSIZE),
    state(BufferState(0)), last(KN_None) {
  }


bool KbdBuffer::enter(int kc, bool in_not_out) {
  auto it = kcs.find(kc);
  if (it==kcs.end())
    return false;

  KeyNumber n = *it;

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

    case KN_FloatDown:
      enter(BC_iFloatDown);
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

    case KN_FloatDown:
      enter(BC_oFloatDown);
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
  data.push_back(b);
}

void KbdBuffer::putback(BufferCode b) {
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

QList<int> KbdBuffer::allKeyCodes() const {
  QList<int> res;
  for (auto it: maprange(kcs))
    res << it.key();
  return res;
}
