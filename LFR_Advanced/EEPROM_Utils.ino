/**
 * @file EEPROM_Utils.ino
 * @brief Manages all interactions with the EEPROM.
 * Includes loading all settings, loading the path, and clearing memory sections.
 */

/**
 * @brief Loads all configuration parameters from EEPROM into global variables.
 */
void eeprom_refresh() {
  // Load sensor calibration data
  for (byte i = 0; i < 6; i++) {
    threshold[i] = EEPROM.read(i) * 4;
    maximum[i] = EEPROM.read(i + 6) * 4;
    minimum[i] = EEPROM.read(i + 12) * 4;
  }
  // Load tuning parameters
  counter = EEPROM.read(19);
  speed = EEPROM.read(20);
  error = EEPROM.read(21) - 25; // Error is stored with a -25 offset
  turn_speed = EEPROM.read(22);
  turn_brake = EEPROM.read(23);
  brake_time = EEPROM.read(24) * 10; // Some values are scaled
  turn90_delay = EEPROM.read(25) * 10;
  u_turn_timer = EEPROM.read(26) * 10;
  stop_timer = EEPROM.read(27);
  node_delay = EEPROM.read(28);
  obstacle_distance = EEPROM.read(29);
  wall_distance = EEPROM.read(30);
  wall_mid = EEPROM.read(31);
  wallp = EEPROM.read(32);
  i_timer = EEPROM.read(33);

  // Calculate speed coefficients based on the error setting
  (error > 0) ? spr = speed - error : spr = speed;
  (error < 0) ? spl = speed + error : spl = speed;
}

/**
 * @brief Loads the stored path from EEPROM into the global path array.
 */
void path_load() {
  for (byte i = 0; i < 25; i++)
    path[i] = EEPROM.read(i + memory);
}

/**
 * @brief Clears the path section of the EEPROM.
 */
void path_clear() {
  oled.set2X();
  oled.clear();
  text("You Sure?", 11, 3);
  oled.set1X();
  text("Long Press to Confirm", 1, 7);
  while (1) {
    byte r = push(SW_PIN);
    if (r == 1) break; // Cancel on short press
    else if (r == 2) { // Confirm on long press
      oled.set2X();
      oled.clear();
      text("  DONE!  ", 11, 3);
      for (byte i = 0; i < 25; i++) {
        EEPROM.write(i + memory, 0);
        delay(10);
      }
      path_load(); // Reload the now-empty path
      delay(1000);
      break;
    }
  }
}

/**
 * @brief Clears a larger portion of the EEPROM, including settings.
 */
void memory_clear() {
  oled.set2X();
  oled.clear();
  text("You Sure?", 11, 3);
  oled.set1X();
  text("Long Press to Confirm", 1, 7);
  while (1) {
    byte r = push(SW_PIN);
    if (r == 1) break; // Cancel
    else if (r == 2) { // Confirm
      oled.set2X();
      oled.clear();
      text("  DONE!  ", 11, 3);
      for (byte i = 0; i < 100; i++) { // Clear first 100 bytes
        EEPROM.write(i, 0);
        delay(10);
      }
      path_load();
      eeprom_refresh(); // Reload all the zeroed-out settings
      delay(1000);
      break;
    }
  }
}
