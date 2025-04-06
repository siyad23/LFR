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

#if (SH1106_LCDHEIGHT != 64)
#error ("Height incorrect, please fix Adafruit_SH1106.h!");
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

void testdrawcircle(void)
{
    for (int16_t i = 0; i < display.height(); i += 2)
    {
        display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
        display.display();
    }
}

void testfillrect(void)
{
    uint8_t color = 1;
    for (int16_t i = 0; i < display.height() / 2; i += 3)
    {
        // alternate colors
        display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
        display.display();
        color++;
    }
}

void testdrawtriangle(void)
{
    for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5)
    {
        display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                             display.width() / 2 - i, display.height() / 2 + i,
                             display.width() / 2 + i, display.height() / 2 + i, WHITE);
        display.display();
    }
}

void testfilltriangle(void)
{
    uint8_t color = WHITE;
    for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5)
    {
        display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                             display.width() / 2 - i, display.height() / 2 + i,
                             display.width() / 2 + i, display.height() / 2 + i, WHITE);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        display.display();
    }
}

void testdrawroundrect(void)
{
    for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
    {
        display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, WHITE);
        display.display();
    }
}

void testfillroundrect(void)
{
    uint8_t color = WHITE;
    for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
    {
        display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        display.display();
    }
}

void testdrawrect(void)
{
    for (int16_t i = 0; i < display.height() / 2; i += 2)
    {
        display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
        display.display();
    }
}

void testdrawline()
{
    for (int16_t i = 0; i < display.width(); i += 4)
    {
        display.drawLine(0, 0, i, display.height() - 1, WHITE);
        display.display();
    }
    for (int16_t i = 0; i < display.height(); i += 4)
    {
        display.drawLine(0, 0, display.width() - 1, i, WHITE);
        display.display();
    }
    delay(250);

    display.clearDisplay();
    for (int16_t i = 0; i < display.width(); i += 4)
    {
        display.drawLine(0, display.height() - 1, i, 0, WHITE);
        display.display();
    }
    for (int16_t i = display.height() - 1; i >= 0; i -= 4)
    {
        display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
        display.display();
    }
    delay(250);

    display.clearDisplay();
    for (int16_t i = display.width() - 1; i >= 0; i -= 4)
    {
        display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
        display.display();
    }
    for (int16_t i = display.height() - 1; i >= 0; i -= 4)
    {
        display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
        display.display();
    }
    delay(250);

    display.clearDisplay();
    for (int16_t i = 0; i < display.height(); i += 4)
    {
        display.drawLine(display.width() - 1, 0, 0, i, WHITE);
        display.display();
    }
    for (int16_t i = 0; i < display.width(); i += 4)
    {
        display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
        display.display();
    }
    delay(250);
}

void printFloat(int x, int y, int size, float num)
{
    display.setTextSize(size);
    display.setCursor(x, y);
    display.print(num, 2); // Print float with 2 decimal places
    display.display();
}