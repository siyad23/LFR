#include "init.h"

void setup()
{
  basic_init();  // Call the initialization function
  switch_init(); // Initialize the rotary encoder and switch
  // display_init();
}

void loop()
{
  display_update();
  // display_update(); // Update the display
}