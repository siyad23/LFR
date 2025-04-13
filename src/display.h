#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

const unsigned int MENU_ITEM = 3;
const unsigned int MENU_ITEM_HEIGHT = 22;
const unsigned int TEXT_OFFSET = 13;
const unsigned int LOGO_OFFSET = 2;

typedef struct
{
    String name;
    logo_t *logo; // Pointer to the logo data
    int *variable;
} menu_item;

menu_item menu[MENU_ITEM] = {
    {"Calibrate", &calibrate_logo, NULL},
    {"Speed", &speed_logo, NULL},
    {"DMP", &DMP_logo, NULL}};

void display_menu(int pos);

void display_init()
{
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);

    display.clearDisplay();
    display.setTextColor(1);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.setFont(&Org_01);
    display.setCursor(19, 40);

    // INITIALIZATION SCREEN
    display.print("LFR");
    display.display();
    delay(2000); // Pause for 2 seconds to show the logo
    display.clearDisplay();
    display_menu(counter); // Display the menu with the first item selected
    display.display();
}

void display_menu(int pos)
{
    display.clearDisplay();

    display.setTextColor(1);
    display.setTextSize(2);

    for (int i = 0; i < MENU_ITEM; i++)
    {
        display.drawBitmap(9, (MENU_ITEM_HEIGHT * i) + LOGO_OFFSET, menu[i].logo->logo_bit, menu[i].logo->width, menu[i].logo->height, 1);
        display.setCursor(28, TEXT_OFFSET + (i * MENU_ITEM_HEIGHT));
        display.print(menu[i].name);
    }

    // Position of the rectangle
    pos = MENU_ITEM_HEIGHT * pos;
    display.drawRect(1, pos, 127, 20, 1);

    display.display();
}