/**
 * @file Sensors.ino
 * @brief Manages all sensor reading and processing.
 * Includes IR array reading, calibration, inverse line detection,
 * and ultrasonic sensor polling.
 */

/**
 * @brief Reads all 6 IR sensors and converts to digital values.
 * Updates global 'sensor' and 'sum' variables.
 * Inverts logic if 'i_mode' is active.
 */
void reading() {
  sensor = 0;
  sum = 0;
  for (byte i = 0; i < 6; i++) {
    // Read from the pin defined in the ir_pins array
    s[i] = analogRead(ir_pins[i]);
    // Compare to threshold and XOR with i_mode to handle normal/inverse lines
    (s[i] > threshold[i]) ? s[i] = 1 ^ i_mode : s[i] = 0 ^ i_mode;
    sensor += s[i] * base[i];
    sum += s[i];
  }
}

/**
 * @brief Detects the special track segment for toggling inverse line mode.
 */
void i_detection() {
  if (s[5] && s[0]) { // Check for the two outermost sensors
    uint32_t timer = millis();
    while ((s[5] || s[0]) && sum < 6) {
      reading();
      if (millis() - timer > i_timer) {
        i_mode = !i_mode; // Toggle inverse mode
        digitalWrite(LED_PIN, i_mode);
        cross = 's'; // Reset turn flags
        // If we are returning to normal mode, advance path counter
        if (!i_mode) {
          (path[counter] == 14) ? counter += 2 : counter++;
          digitalWrite(LED_PIN, HIGH);
          m1 = millis();
        }
        break; // Exit the while loop
      }
    }
    turn = side; // Set a guided turn for the Y-junction after detection
  }
}

/**
 * @brief Runs the sensor calibration routine.
 * Rotates to find min/max values and saves thresholds to EEPROM.
 */
void cal() {
  byte i = 0;
  for (i = 0; i < 6; i++) {
    maximum[i] = 0;
    minimum[i] = 1024;
  }
  motor(-100, 100); // Rotate the robot
  for (short int c = 0; c < 3000 ; c++) {
    for (i = 0; i < 6; i++) {
      // Read from the pin defined in the ir_pins array
      int val = analogRead(ir_pins[i]);
      maximum[i] = max(maximum[i], val);
      minimum[i] = min(minimum[i], val);
    }
    delay(1);
  }
  motor(0, 0);
  for (i = 0; i < 6; i++) {
    // Calculate threshold as the midpoint between min and max
    threshold[i] = (maximum[i] - minimum[i]) * 0.5 + minimum[i];
    // Save calibration data to EEPROM (scaled to fit in a byte)
    EEPROM.write(i, threshold[i] / 4 ); delay(10);
    EEPROM.write(i + 6, maximum[i] / 4 ); delay(10);
    EEPROM.write(i + 12, minimum[i] / 4 ); delay(10);
  }
  eeprom_refresh(); // Reload the new values
}

// --- ULTRASONIC SENSOR READING FUNCTIONS ---

/**
 * @brief Reads the front ultrasonic sensor for wall detection.
 * @param mode If true, forces a new reading. Otherwise, uses cached value.
 * @return True if a wall is within wall_distance, false otherwise.
 */
bool wallf_read(bool mode) {
  if (millis() - tf > 50 || mode) {
    tf = millis();
    sf = sonarf.ping_cm();
    if (sf == 0) sf = MAX_DISTANCE; // Use max distance if no ping
  }
  return (sf < wall_distance);
}

/**
 * @brief Reads the front ultrasonic sensor for obstacle detection.
 * @param mode If true, forces a new reading.
 * @return True if an obstacle is within obstacle_distance.
 */
bool sonarf_read(bool mode) {
  if (millis() - tf > 50 || mode) {
    tf = millis();
    sf = sonarf.ping_cm();
    if (sf == 0) sf = MAX_DISTANCE;
  }
  return (sf < obstacle_distance);
}

/**
 * @brief Reads the right ultrasonic sensor.
 * @param mode If true, forces a new reading.
 * @return True if an object is within wall_limit.
 */
bool sonarr_read(bool mode) {
  if (millis() - tr > 50 || mode) {
    tr = millis();
    sr = sonarr.ping_cm();
    if (sr == 0) sr = MAX_DISTANCE;
  }
  return (sr < wall_limit);
}

/**
 * @brief Reads the left ultrasonic sensor.
 * @param mode If true, forces a new reading.
 * @return True if an object is within wall_limit.
 */
bool sonarl_read(bool mode) {
  if (millis() - tl > 50 || mode) {
    tl = millis();
    sl = sonarl.ping_cm();
    if (sl == 0) sl = MAX_DISTANCE;
  }
  return (sl < wall_limit);
}
