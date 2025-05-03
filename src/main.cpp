#include "init.h"

void setup()
{
  basic_init();  // Call the initialization function
  switch_init(); // Initialize the rotary encoder and switch
  // display_init();
}

void loop()
{
  handleSwitch();        // Handle the switch with debounce
  handleRotaryEncoder(); // Handle the rotary encoder
  // display_update(); // Update the display
}