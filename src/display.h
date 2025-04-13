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

bool requireUpdate = false; // Flag to indicate if the display needs to be updated
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
    {"Speed", &speed_logo, speed_menu},
    {"DMP", &DMP_logo, dmp_menu}};

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
{ // Toggle between menu levels
    bool isRotaryChange = handleRotaryEncoder();
    bool isSwitchChange = handleSwitch(); // Check if the rotary encoder or switch is pressed

    if (isRotaryChange || isSwitchChange) // If either the rotary encoder or switch is pressed
    {
        requireUpdate = true;
        if (isSwitchChange)
            menu_level = !menu_level; // Toggle menu level if switch is pressed
    }

    if (requireUpdate)
    {
        if (menu_level == 0)
        {
            main_menu();
        }
        // Updates the calibration menu
        else if (menu_level == 1)
        {
            menu[menu_index].callingFunction(); // Call the function associated with the selected menu item
        }
    }

    requireUpdate = false; // Reset the update flag
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
    display.drawBitmap(56, 5, calibrate_logo.logo_bit, 16, 16, 1);

    // Layer 2
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(23, 37);
    display.print("Calibrate");

    // Layer 4
    display.setTextSize(1);
    display.setCursor(24, 54);
    display.print("Press to strart");

    display.display();
}

void speed_menu()
{
}

void dmp_menu()
{
}