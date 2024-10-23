
# spreLgfxTouch

`spreLgfxTouch` is an Arduino library designed specifically for the [Sony Spresense](https://developer.sony.com/develop/spresense/) platform. It provides functionality to display JPEG images on an ILI9341 display and handle touch input with an XPT2046 touch controller. The library is built on top of `LovyanGFX` for efficient graphics rendering and `XPT2046_Touchscreen` for touch input.

## Features

- Display JPEG images on ILI9341-based displays.
- Handle touch input with XPT2046 touch controller.
- Support for 16-bit and 8-bit color depth.
- Optimized for Sony Spresense.
- Smooth frame rate display and basic touch drawing functions.

## Requirements

Before using `spreLgfxTouch`, ensure you have the following libraries installed:

- [LovyanGFX](https://github.com/lovyan03/LovyanGFX) - High-performance graphics library for Arduino.
- [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen) - Touchscreen controller library for XPT2046.

Both libraries will be automatically installed if you install `spreLgfxTouch` through the Arduino Library Manager.

## Installation

### Using Arduino Library Manager

1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Manage Libraries...**
3. In the Library Manager, search for `spreLgfxTouch`.
4. Click **Install**.

### Manual Installation

1. Download the `spreLgfxTouch` library from the [GitHub repository](https://github.com/your-repo-url).
2. Extract the downloaded ZIP file.
3. Copy the `spreLgfxTouch` folder into the `libraries` folder of your Arduino directory (usually `Documents/Arduino/libraries`).

## How to Use

### Basic Example

The following example demonstrates how to display a JPEG image on the screen and handle touch input:

```cpp
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
```

### Key Functions

- **`setupLGFX(int16_t cdep, int16_t rot)`**: Initializes the display with the specified color depth (`cdep`) and rotation (`rot`).
- **`setupTouch(int16_t w, int16_t h, int16_t r, bool swap)`**: Sets up the touch controller with the screen dimensions (`w`, `h`), rotation (`r`), and whether to swap the touch axes (`swap`).
- **`isTouch(int *tx, int *ty, int *tz)`**: Detects if the screen is being touched and returns the touch coordinates (`tx`, `ty`) and pressure (`tz`).
- **`drawJpgToMemory(LGFX_Sprite *dst, const char *fname)`**: Draws a JPEG image from the SD card to the specified sprite (`dst`).

## Dependencies

This library depends on the following:

- **LovyanGFX**: For fast graphics rendering on the ILI9341 display.
- **XPT2046_Touchscreen**: For touch input handling.

These dependencies will be automatically installed when using the Arduino Library Manager.

## Hardware Setup

- **Display**: ILI9341-based SPI display (240x320 resolution).
- **Touch Controller**: XPT2046 touch controller via SPI.
- **Microcontroller**: Sony Spresense main board.

### Wiring

| Component       | Spresense Pin   | Display/Touch Pin |
|-----------------|-----------------|-------------------|
| SPI SCLK        | PIN_SPI_SCK     | SCK               |
| SPI MOSI        | PIN_SPI_MOSI    | MOSI              |
| SPI MISO        | PIN_SPI_MISO    | MISO              |
| SPI CS (Display)| 10              | CS (Display)      |
| SPI DC (Display)| 9               | DC                |
| SPI RST         | 8               | RST               |
| SPI CS (Touch)  | 7               | CS (Touch)        |

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

