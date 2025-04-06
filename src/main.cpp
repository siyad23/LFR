#include "init.h"

void setup()
{
  switch_init(); // Initialize the rotary encoder and switch
  Serial.begin(9600);
}

void loop()
{

  handleRotaryEncoder(); // Call the rotary encoder handler
  handleSwitch();        // Call the switch handler
}