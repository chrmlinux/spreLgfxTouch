//
//  name          : spreLGFXLib.hpp
//  date/author   : 2024/10/23 chrmlinux03
//  update/author : 2024/10/23 chrmlinux03
//

#ifndef __SPRELGFX_HPP__
#define __SPRELGFX_HPP__

#define SPI4_SCLK  (13)
#define SPI4_MISO  (12)
#define SPI4_MOSI  (11)
#define SPI4_CS    (10)
#define SPI4_DC    ( 9)
#define SPI4_RST   ( 8)
#define SPI4_CS2   ( 7)

#define IO_SW4    ( 7)
#define IO_SW3    ( 6)
#define IO_SW2    ( 5)
#define IO_SW1    ( 4)

#define ROT0   (0)
#define ROT90  (1)
#define ROT180 (2)
#define ROT270 (3)
#define DEPTH_8BIT (8)
#define DEPTH_16BIT (16)

#define MEGA (1000 * 1000)
#define LGFX_AUTODETECT
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI       _bus_instance;

  public:
    LGFX(void) {
      {
        auto cfg = _bus_instance.config();
        cfg.spi_mode    = 0;
        cfg.spi_port    = 4;
        cfg.freq_write  = (20 * MEGA);
        cfg.freq_read   = (16 * MEGA);
//        cfg.freq_write  = (24 * MEGA);
//        cfg.freq_read   = (24 * MEGA);
        cfg.pin_dc      = SPI4_DC;
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();
        cfg.pin_cs           =  SPI4_CS;
        cfg.pin_rst          =  SPI4_RST;
        cfg.pin_busy         =  -1;
        cfg.panel_width      =   240;
        cfg.panel_height     =   320;
        cfg.bus_shared       =  true;
        _panel_instance.config(cfg);
      }

      setPanel(&_panel_instance);
    }
};

static LGFX lcd;
static LGFX_Sprite spr;
static bool useGraphics = false;
static uint16_t _w, _h, _hw, _hh;

int16_t setupLGFX(int16_t cdep, int16_t rot) {
  lcd.init();
  lcd.setRotation(rot);
  spr.setColorDepth(cdep);
  spr.createSprite(lcd.width(), lcd.height());
  _w = spr.width(); _h = spr.height();
  _hw = _w >> 1; _hh = _h >> 1;
  useGraphics = true;
  return 0;
}


//==================================
// getfps
//==================================
uint32_t getfps(void) {

  static uint32_t psec = 0;
  static uint32_t cnt = 0;
  static uint32_t fps = 0;
  uint32_t sec = 0;

  sec = millis() / 1000;
  ++cnt;
  if (psec != sec) {
    psec = sec;
    fps = cnt;
    cnt = 0;
  }
  return fps;
}

//==================================
// drawAst
//==================================
void drawAst(LGFX_Sprite *dst, int gx, int gy, int fgcol, int bgcol) {
  static int cnt = 0;
  char ast[5] = "|/-\\";
	dst->setTextColor(fgcol, bgcol);
  dst->setCursor(gx, gy);
  dst->printf("[%c]%3dfps\n", ast[cnt], getfps());
  cnt = (cnt + 1) % 4;
}

//==================================
// drawFparam
//==================================
void drawFparam(LGFX_Sprite *spr, int gx, int gy, char *fmt, float fdt, int fgcol, int bgcol) {
	spr->setTextColor(fgcol, bgcol);
  spr->setCursor(gx, gy);
  spr->printf(fmt, fdt);
}

//==================================
// drawIparam
//==================================
void drawIparam(LGFX_Sprite *spr, int gx, int gy, char *fmt, uint32_t dt, int fgcol, int bgcol) {
	spr->setTextColor(fgcol, bgcol);
  spr->setCursor(gx, gy);
  spr->printf(fmt, dt);
}

//==================================
// drawSparam
//==================================
void drawSparam(LGFX_Sprite *spr, int gx, int gy, char *fmt, int fgcol, int bgcol) {
	spr->setTextColor(fgcol, bgcol);
  spr->setCursor(gx, gy);
  spr->print(fmt);
}

//==============================
// drawCorner
//==============================
void drawCorner(LGFX_Sprite *dst, int rectX, int rectY, int rectWidth, int rectHeight, int col) {
    int cSize = 16;
    dst->drawLine(rectX, rectY, rectX + cSize, rectY, col);
    dst->drawLine(rectX, rectY, rectX, rectY + cSize, col);
    dst->drawLine(rectX + rectWidth - cSize, rectY, rectX + rectWidth, rectY, col);
    dst->drawLine(rectX + rectWidth, rectY, rectX + rectWidth, rectY + cSize, col);
    dst->drawLine(rectX, rectY + rectHeight - cSize, rectX, rectY + rectHeight, col);
    dst->drawLine(rectX, rectY + rectHeight, rectX + cSize, rectY + rectHeight, col);
    dst->drawLine(rectX + rectWidth - cSize, rectY + rectHeight, rectX + rectWidth, rectY + rectHeight, col);
    dst->drawLine(rectX + rectWidth, rectY + rectHeight - cSize, rectX + rectWidth, rectY + rectHeight, col);
}

//==================================
// drawJpgToMemory
//==================================
int drawJpgToMemory(LGFX_Sprite *dst, const char *fname) {
  static LGFX_Sprite jpg;
  static bool jpgLoaded = false;
  int rtn = 0;
  uint32_t sz = _w * _h;

  if (!jpgLoaded) {
    jpg.setColorDepth(DEPTH_8BIT);
    if (!jpg.createSprite(_w, _h)) {
      Serial.printf("ERROR: malloc error (jpg:%dByte)\n", sz);
      rtn = -1;
    }
    jpg.drawJpgFile(SD, fname, 0, 0);
    jpgLoaded = true;
  }
  jpg.pushSprite(dst, 0, 0);
  return rtn;
}


#endif
