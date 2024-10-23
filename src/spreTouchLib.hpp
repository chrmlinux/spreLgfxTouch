//
//  name          : spreTouchLib.hpp
//  date/author   : 2024/10/23 chrmlinux03
//  update/author : 2024/10/23 chrmlinux03
//

#ifndef __SPRETOUCH_HPP__
#define __SPRETOUCH_HPP__

#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen tch(SPI4_CS2);

static uint16_t _tx, _ty, _tz;
static uint16_t _tw, _th, _tr;
static uint16_t _thw, _thh;
static bool _swap = 0;
static bool useTouch = false;

#define HARDWARETOUCH_W (390)
#define HARDWARETOUCH_H (3900)

#define LCD24 true
#define LCD28	false

//int16_t isTouch(void) {
int16_t isTouch(int *tx, int *ty, int *tz) {
  if (!tch.touched()) {
    return 0;
  }

  TS_Point p = tch.getPoint();
  _tz = p.z;

  int txmin =   0;
  int txmax = _tw;
  if (_swap) {
    txmin = _tw;
    txmax =   0;
  }

  switch (_tr) {
    case 0:
      _tx = map(p.y, _thh, _thw, txmin, txmax);
      _ty = map(p.x, _thw, _thh, 0, _th);
      break;
    case 1:
      _tx = map(p.x, _thw, _thh, txmin, txmax);
      _ty = map(p.y, _thw, _thh, 0, _th);
      break;
    case 2:
      _tx = map(p.y, _thw, _thh, txmin, txmax);
      _ty = map(p.x, _thh, _thw, 0, _th);
      break;
    case 3:
      _tx = map(p.x, _thh, _thw, txmin, txmax);
      _ty = map(p.y, _thh, _thw, 0, _th);
      break;
  }

	*tx = _tx;
	*ty = _ty;
	*tz = _tz;
  return 1;
}

void setupTouch(int16_t w, int16_t h, int16_t r, bool swap) {
  _tw = w;
  _th = h;
  _tr = r;
  _thw = HARDWARETOUCH_W;
  _thh = HARDWARETOUCH_H;  
  _swap = swap;
  tch.begin();
}

#endif
