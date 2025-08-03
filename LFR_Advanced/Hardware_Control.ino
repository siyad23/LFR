/**
 * @file Hardware_Control.ino
 * @brief Contains low-level functions for controlling hardware.
 * This includes motor driving, braking, and reading the rotary encoder and button.
 */

/**
 * @brief Controls the speed and direction of both motors.
 * @param a Speed for the left motor (-255 to 255).
 * @param b Speed for the right motor (-255 to 255).
 */
void motor(int a, int b) {
  // Left Motor
  if (a >= 0) {
    analogWrite(LEFT_MOTOR_FORWARD, a);
    analogWrite(LEFT_MOTOR_BACKWARD, 0);
  } else {
    a = -a;
    analogWrite(LEFT_MOTOR_FORWARD, 0);
    analogWrite(LEFT_MOTOR_BACKWARD, a);
  }
  // Right Motor
  if (b >= 0) {
    analogWrite(RIGHT_MOTOR_FORWARD, b);
    analogWrite(RIGHT_MOTOR_BACKWARD, 0);
  } else {
    b = -b;
    analogWrite(RIGHT_MOTOR_FORWARD, 0);
    analogWrite(RIGHT_MOTOR_BACKWARD, b);
  }
}

/**
 * @brief Applies brakes by reversing motors briefly.
 */
void brake() {
  motor(-250, -250);
  delay(brake_time);
  motor(0, 0);
  delay(100);
}

/**
 * @brief Performs a timed 90-degree turn.
 * @param s The direction to turn ('l' for left, 'r' for right).
 */
void turn_90(char s) {
  (s == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
  delay(turn90_delay);
  (s == 'r') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
  delay(turn_brake); // Apply a counter-turn to stop cleanly
  motor(0, 0);
  delay(100);
}

/**
 * @brief Reads the state of the rotary encoder.
 * @return 1 for clockwise, 2 for counter-clockwise, 0 for no change.
 */
byte encoder() {
  bool newClk = digitalRead(ENCODER_CLK);
  if (newClk != lastClk) {
    lastClk = newClk;
    bool dtValue = digitalRead(ENCODER_DT);
    if (newClk == LOW && dtValue == HIGH) return 1; // Clockwise
    if (newClk == LOW && dtValue == LOW) return 2;  // Counter-clockwise
  }
  return 0;
}

/**
 * @brief Reads a button press and distinguishes between short and long presses.
 * @param button_pin The pin number of the button.
 * @return 1 for short press, 2 for long press, 0 for no press.
 */
byte push(byte button_pin) {
  uint32_t b_timer = 0;
  if (!digitalRead(button_pin)) { // If button is pressed
    delay(50); // Debounce
    while (!digitalRead(button_pin)) {
      delay(50);
      b_timer += 50;
      if (b_timer > 1000) digitalWrite(LED_PIN, 1); // Turn on LED for long press
    }
    digitalWrite(LED_PIN, 0);
    if (b_timer > 1000) return 2; // Long press
    else if (b_timer > 0) return 1; // Short press
  }
  return 0;
}
Hardware_Control.ino