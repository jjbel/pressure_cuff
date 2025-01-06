#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
// https://youtu.be/4DtuOeeYHys
// our display also needs reset pin

struct Plotter {
  Adafruit_ILI9341 &tft;

  int16_t box_x;
  int16_t box_y; // +ve y upward
  int16_t box_w;
  int16_t box_h;

  float in_y_max;
  int16_t out_x = 0;

  void plot(float y) {
    tft.drawPixel(box_x + out_x,
                  box_y + box_h - static_cast<int16_t>(y / in_y_max * box_h),
                  ILI9341_WHITE);

    refresh();
  }

  void refresh() {
    out_x++;
    if (out_x >= box_w) {
      out_x = 0;
      tft.fillRect(box_x, box_y, box_w, box_h, ILI9341_BLACK);
      Serial.println("refresh");
    }
  }
};

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Plotter plt = Plotter{tft, 5, 150, 300, 80, 80};

void setup() {
  Serial.begin(38400);
  Serial.println("ILI9341 Test");

  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x");
  Serial.println(x, HEX);

  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  tft.setTextSize(2);
  tft.setCursor(0, 0);

  // since this text is static only draw it once
  tft.println("session dur min");
  tft.println("ramp up sec");
  tft.println("steady high sec");
  tft.println("ramp down sec");
  tft.println("steady high sec");
}

int i = 0;

void loop(void) {
  tft.fillRect(200, 0, 100, 200, ILI9341_BLACK);
  tft.setCursor(200, 0);
  tft.print(i);
  tft.setCursor(200, 16);
  tft.print(i + 1);
  tft.setCursor(200, 32);
  tft.print(i + 2);
  tft.setCursor(200, 48);
  tft.print(i + 3);
  tft.setCursor(200, 64);
  tft.print(i + 4);

  plt.plot(40 + 40 * sin(i / 50.0));

  //   delay(100);
  i++;
}
