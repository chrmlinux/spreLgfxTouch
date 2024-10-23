//
//  name          : spreLgfxTouch.ino
//  date/author   : 2024/10/23 chrmlinux03
//  update/author : 2024/10/23 chrmlinux03
//

#include <SDHCI.h>
#include <File.h>
SDClass SD;
File myFile;

#include "spreLGFXLib.hpp"
#include "spreTouchLib.hpp"

//===================================
// setup
//===================================
void setup(void) {
  Serial.begin(115200);
  if (!SD.begin()) {
    Serial.println("ERROR: SD card");
    while (1);
  }
  setupLGFX(DEPTH_16BIT, ROT90);
  setupTouch(_w, _h, ROT90, false);
}

//===================================
// loop
//===================================
void loop(void) {
  drawJpgToMemory(&spr, "/sample.jpg");
  drawCorner(&spr, _hw/2, _hh/2, _hw, _hh, TFT_WHITE);
  int tx, ty, tz;
  if (isTouch(&tx, &ty, &tz)) {
    spr.drawLine(tx, 0, tx, _h, TFT_GREEN);
    spr.drawLine(0, ty, _w, ty, TFT_BLUE);
    spr.drawCircle(tx, ty, tz / 100, TFT_RED);
  }
  drawAst(&spr, 0, _h - 8, TFT_BLACK, TFT_WHITE);
  spr.pushSprite(&lcd, 0, 0);
}
