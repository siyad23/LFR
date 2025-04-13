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

unsigned int menu_index = 0;
unsigned int menu_level = 0;

void main_menu();
void calibrate_menu();

typedef void (*FunctionType)();
typedef struct
{
    String name;
    logo_t *logo;                 // Pointer to the logo data
    FunctionType callingFunction; // Pointer to the function to call
    // FunctionType function; // Pointer to the function to call
} menu_item;

menu_item menu[MENU_ITEM] = {
    {"Calibrate", &calibrate_logo, calibrate_menu},
    {"Speed", &speed_logo, NULL},
    {"DMP", &DMP_logo, NULL}};

void main_menu();
void calibrate_menu();

void display_init()
{
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);

    // Basic initialization
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

    // Display the menu with the first item selected
    main_menu();
    display.display();
}

void display_update(void)
{
    if (handleSwitch())
        menu_level = !menu_level; // Toggle between menu levels

    // Updates the main menu
    if (menu_level == 0)
    {
        if (handleRotaryEncoder())
            main_menu();
    }
    // Updates the calibration menu
    else if (menu_level == 1)
    {
        menu[menu_index].callingFunction(); // Call the function associated with the selected menu item
    }
}

void main_menu()
{
    menu_index = counter % MENU_ITEM;
    if (menu_index < 0)
    {
        menu_index = MENU_ITEM - 1; // Wrap around to the last menu item
    }

    display.clearDisplay();
    display.setTextColor(1);
    display.setTextSize(2);

    for (unsigned int i = 0; i < MENU_ITEM; i++)
    {
        display.drawBitmap(9, (MENU_ITEM_HEIGHT * i) + LOGO_OFFSET, menu[i].logo->logo_bit, menu[i].logo->width, menu[i].logo->height, 1);
        display.setCursor(28, TEXT_OFFSET + (i * MENU_ITEM_HEIGHT));
        display.print(menu[i].name);
    }

    // Position of the rectangle
    display.drawRect(1, MENU_ITEM_HEIGHT * menu_index, 127, 20, 1);
    display.display();
}

void calibrate_menu()
{
    display.clearDisplay();

    // menu
    display.drawBitmap(56, 5, calibrate_logo.logo_bit, calibrate_logo.width, calibrate_logo.height, 1);

    // Layer 2
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(11, 25);
    display.print("Calibrate");

    // Layer 4
    display.setTextSize(1);
    display.setCursor(20, 48);
    display.print("Press to strart");

    display.display();
}