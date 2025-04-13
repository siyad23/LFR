#include "init.h"

void setup()
{
  basic_init();  // Call the initialization function
  switch_init(); // Initialize the rotary encoder and switch
  display_init();
}

void loop()
{
  if (handleRotaryEncoder()) // Check if the rotary encoder is rotated
  {
    counter = counter % MENU_ITEM;
    if (counter < 0)
    {
      counter = MENU_ITEM - 1; // Wrap around to the last menu item
    }
    display_menu(counter); // Call the draw function with the current counter value
  }
}