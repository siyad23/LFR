#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

const int MENU_ITEM = 3;

const int MENU_ITEM_WIDTH = 22; // Width of each menu item in pixels

#if (SH1106_LCDHEIGHT != 64)
Serial.print("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

void display_init()
{
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.println(F("LFR Test"));
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
}

void draw(int pos)
{
    display.clearDisplay();

    // Layer 1
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setFont(&Org_01);
    display.setCursor(28, 13);
    display.print("Calibrate");

    // Layer 2
    display.setCursor(28, 35);
    display.print("Speed");

    // Layer 3
    display.setCursor(28, 57);
    display.print("DMP");

    // menu
    display.drawBitmap(9, 2, image_menu_bits, 16, 16, 1);

    // download
    display.drawBitmap(9, 24, image_download_bits, 15, 16, 1);

    // file_share
    display.drawBitmap(8, 47, image_file_share_bits, 16, 14, 1);

    pos = 22 * pos;
    // Layer 7
    display.drawRect(1, pos, 127, 20, 1);

    display.display();
}

void printFloat(int x, int y, int size, float num)
{
    display.setTextSize(size);
    display.setCursor(x, y);
    display.print(num, 2); // Print float with 2 decimal places
    display.display();
}