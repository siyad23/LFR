#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <EEPROM.h>
#include <NewPing.h>
SSD1306AsciiWire oled;

NewPing sonarf(7, 7, 30);
NewPing sonarl(8, 8, 30);
NewPing sonarr(9, 9, 30);

#define left_motor_forward 5
#define left_motor_backward 6
#define right_motor_forward 10
#define right_motor_backward 11
#define ENCODER_CLK 2
#define ENCODER_DT 3
#define sw 4
#define led 13
bool lastClk = HIGH;

int speed, error, spl, spr, turn_speed;
int brake_time, turn_brake, turn90_delay, node_delay, stop_timer, u_turn_timer, i_timer;
int obstacle_distance, wall_distance, wall_mid, wallp, wall_limit = 25;
char side = 'r', turn = 's', cross = 's';
int pos;

int sensor, sum;
int s[6], base[6] = {1, 2, 4, 8, 16, 32};
int threshold[6], maximum[6], minimum[6];
bool i_mode = 0;
int sf, sl, sr;

byte path[25] = {1, 14};
byte memory = 50;
int counter;

uint32_t m1, m2, tf, tr, tl;

void setup()
{
  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(led, OUTPUT);
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(System5x7);
  eeprom_refresh();
  path_load();
  TechTopia();
}

void loop()
{
  byte r = push(sw);
  if (r)
  {
    if (r == 1)
    {
      r = menu();
      if (r)
      {
        if (r == 1)
          line_follow();
        else if (r == 2)
          counter_adjust();
        else if (r == 3)
          adjustment_panel();
        else if (r == 4)
          path_panel();
        else if (r == 5)
          calibration_display();
        else if (r == 6)
          analog_display();
        else if (r == 7)
          digital_display();
        else if (r == 8)
        {
          motor(10 * spl, 10 * spr);
          while (!push(sw))
            ;
        }
        else if (r == 9)
          turn_90('l');
        else if (r == 10)
          path_clear();
        else if (r == 11)
          memory_clear();
      }
    }
    TechTopia();
  }
}
