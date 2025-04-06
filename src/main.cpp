#include "init.h"

void setup()
{
  basic_init();  // Call the initialization function
  switch_init(); // Initialize the rotary encoder and switch
  display_init();
}

void loop()
{
  if (handleRotaryEncoder()) // Call the rotary encoder handler and check if it returns true
  {
    display.clearDisplay();      // Clear the display
    display.setTextSize(1);      // Set text size to 1
    display.setTextColor(WHITE); // Set text color to white
    display.setCursor(0, 0);
    display.print("Counter:"); // Set cursor position to (0, 0)
    display.println(counter);  // Print the counter value to the serial monitor
    display.display();
  }

  if (handleSwitch()) // Call the switch handler and check if it returns true
  {
    display.clearDisplay();            // Clear the display
    display.setTextSize(1);            // Set text size to 1
    display.setTextColor(WHITE);       // Set text color to white
    display.setCursor(0, 0);           // Set cursor position to (0, 0)
    display.println("Switch Pressed"); // Print the counter value to the serial monitor
    display.display();                 // Print message to the serial monitor
  }
}