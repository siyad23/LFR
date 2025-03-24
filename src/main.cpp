#include "init.h"

void setup()
{
  Serial.begin(9600);

  display_init();

  display.drawPixel(10, 10, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hello, world!");
  display.println("This is a test of the display");
  display.display();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
