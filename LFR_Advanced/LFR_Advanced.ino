/**
 * @file LFR_Advanced.ino
 * @author Khandaker Siam Ahmed
 * @brief Main file for the Advanced Line Following Robot.
 * This version includes an OLED display, rotary encoder, ultrasonic sensors,
 * and a full menu system for on-the-fly configuration.
 * It handles the main setup and loop, delegating tasks to other files.
 */

// Include required libraries
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <EEPROM.h>
#include <NewPing.h>

// Create an instance of the OLED display
SSD1306AsciiWire oled;

// ------------------- HARDWARE DEFINITIONS -------------------
// --- IR Sensor Pin Mapping to resolve I2C conflict ---
const byte ir_pins[6] = {A0, A1, A2, A3, A6, A7}; // Using A6, A7 to free A4, A5 for OLED

// Ultrasonic Sensor Pins (Trigger and Echo are the same)
#define SONAR_F_PIN 7
#define SONAR_L_PIN 8
#define SONAR_R_PIN 9
#define MAX_DISTANCE 30 // Maximum distance for ultrasonic sensors (in cm)

// Motor Driver Pins
#define LEFT_MOTOR_FORWARD 5
#define LEFT_MOTOR_BACKWARD 6
#define RIGHT_MOTOR_FORWARD 10
#define RIGHT_MOTOR_BACKWARD 11

// Rotary Encoder and Switch Pins
#define ENCODER_CLK 2
#define ENCODER_DT 3
#define SW_PIN 4

// Onboard LED Pin
#define LED_PIN 13

// ------------------- GLOBAL VARIABLES & CONSTANTS -------------------

// Create instances for the ultrasonic sensors
NewPing sonarf(SONAR_F_PIN, SONAR_F_PIN, MAX_DISTANCE);
NewPing sonarl(SONAR_L_PIN, SONAR_L_PIN, MAX_DISTANCE);
NewPing sonarr(SONAR_R_PIN, SONAR_R_PIN, MAX_DISTANCE);

// --- Navigation & Control ---
int speed, error, spl, spr, turn_speed;
char side = 'r', turn = 's', cross = 's';
int pos;
bool i_mode = 0; // 0 = black line on white, 1 = white line on black

// --- Path Following ---
byte path[25] = { 1, 14 }; // Default path, can be changed via menu
byte memory = 50; // Starting address in EEPROM for path data
int counter;

// --- Timers & Delays ---
int brake_time, turn_brake, turn90_delay, node_delay, stop_timer, u_turn_timer, i_timer;
uint32_t m1, m2, tf, tr, tl; // General purpose and sonar timers

// --- Sensor Data ---
int sensor, sum;
int s[6]; // Digital sensor readings (0 or 1)
int base[6] = { 1, 2, 4, 8, 16, 32 }; // For binary to decimal conversion
int threshold[6], maximum[6], minimum[6]; // Calibration values
int sf, sl, sr; // Stores distances from ultrasonic sensors

// --- Wall & Obstacle Detection ---
int obstacle_distance, wall_distance, wall_mid, wallp, wall_limit = 25;

// --- Rotary Encoder State ---
bool lastClk = HIGH;

void setup() {
  // Initialize pin modes
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize I2C communication for the OLED
  Wire.begin();
  Wire.setClock(400000L);

  // Initialize the OLED display
  // Use Sh1106_128x64 for 1.3" OLEDs with the SH1106 driver.
  oled.begin(&Sh1106_128x64, 0x3C);
  
  oled.setFont(System5x7);

  // Load all settings and calibration data from EEPROM
  eeprom_refresh();
  // Load the stored path from EEPROM
  path_load();
  // Display the splash screen
  TechTopia();
}

void loop() {
  // Check if the main button is pressed
  byte r = push(SW_PIN);
  if (r) {
    // A short press enters the main menu
    if (r == 1) {
      byte menu_choice = menu();
      if (menu_choice) {
        // Execute the function chosen from the menu
        if (menu_choice == 1) line_follow();
        else if (menu_choice == 2) counter_adjust();
        else if (menu_choice == 3) adjustment_panel();
        else if (menu_choice == 4) path_panel();
        else if (menu_choice == 5) calibration_display();
        else if (menu_choice == 6) analog_display();
        else if (menu_choice == 7) digital_display();
        else if (menu_choice == 8) {
          // Test motors: go forward until button is pressed again
          motor(10 * spl, 10 * spr);
          while (!push(SW_PIN));
          motor(0, 0);
        } else if (menu_choice == 9) {
            // Test 90-degree turns
            turn_90('l');
            delay(500);
            turn_90('r');
        }
        else if (menu_choice == 10) path_clear();
        else if (menu_choice == 11) memory_clear();
      }
    }
    // After any action, return to the splash screen
    TechTopia();
  }
}
