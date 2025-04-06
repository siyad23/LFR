#include <Arduino.h>
#include "display.h"
#include "switch.h"

void basic_init()
{                       // Initialize the rotary encoder and switch
    Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}