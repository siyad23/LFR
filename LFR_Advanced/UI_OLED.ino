/**
 * @file UI_OLED.ino
 * @brief Handles all User Interface functions for the OLED display.
 * This includes the main menu, parameter adjustment panels, path editor,
 * and various display/debug screens.
 */

// --- MENU NAVIGATION ---

/**
 * @brief Displays the main menu and handles navigation.
 * @return The selected menu item number (1-11), or 0 if exited.
 */
byte menu() {
  oled.set1X();
  oled.clear();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 11;

  while (1) {
    // Redraw menu only when the selection changes
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8 ; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        text(String(i + base) + ") " + menu_list(i + base), 0, i);
      }
    }

    // Handle encoder input for scrolling
    byte enc = encoder();
    if (enc) {
      if (enc == 1) { // Clockwise
        ind++;
        if (ind > limit) { ind = 1; }
      } else { // Counter-clockwise
        ind--;
        if (ind < 1) { ind = limit; }
      }
      // Adjust the visible window of the menu
      if (ind > peak) { peak = ind; base = peak - 7; }
      else if (ind < base) { base = ind; peak = base + 7; }
    }

    // Handle button press for selection/exit
    byte r = push(SW_PIN);
    if (r) {
      if (r == 1) return ind; // Short press: select
      else return 0;         // Long press: exit
    }
  }
}

/**
 * @brief Returns the string for a given menu item.
 * @param a The menu item number.
 * @return The corresponding menu text.
 */
String menu_list(byte a) {
  if (a == 1)     return "Line Follow";
  else if (a == 2)  return "Counter";
  else if (a == 3)  return "Adjustment";
  else if (a == 4)  return "Path Adjust";
  else if (a == 5)  return "Calibration";
  else if (a == 6)  return "Analog Display";
  else if (a == 7)  return "Digital Display";
  else if (a == 8)  return "Go Forward";
  else if (a == 9)  return "Rotation Test";
  else if (a == 10) return "Path Clear";
  else if (a == 11) return "Memory Clear";
  return "INVALID";
}


// --- ADJUSTMENT PANELS ---

/**
 * @brief Displays the panel for adjusting robot parameters.
 */
void adjustment_panel() {
  oled.set1X();
  oled.clear();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 14;
  int value;

  while (1) {
    // Redraw panel only when selection changes
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        // Some values are scaled by 10 in EEPROM
        if (i + base >= 5 && i + base <= 7) value = EEPROM.read(i + base + 19) * 10;
        else value = EEPROM.read(i + base + 19);
        
        // Motor error is stored with an offset
        if (i + base == 2) text(String(i + base) + ") " + adjustment_list(i + base) + String(value - 25), 0, i);
        else text(String(i + base) + ") " + adjustment_list(i + base) + String(value), 0, i);
      }
    }

    // Handle encoder input for scrolling
    byte enc = encoder();
    if (enc) {
      if (enc == 1) { // Clockwise
        ind++;
        if (ind > limit) { ind = 1; }
      } else { // Counter-clockwise
        ind--;
        if (ind < 1) { ind = limit; }
      }
      if (ind > peak) { peak = ind; base = peak - 7; }
      else if (ind < base) { base = ind; peak = base + 7; }
    }

    // Handle button press for selection/exit
    byte r = push(SW_PIN);
    if (r) {
      if (r == 1) set_adjust(ind); // Short press: edit value
      else {                       // Long press: exit and reload settings
        eeprom_refresh();
        return;
      }
      temp = 0; // Force redraw after returning from editing
    }
  }
}

/**
 * @brief Allows editing a single parameter value.
 * @param indicator The index of the parameter to edit.
 */
void set_adjust(int indicator) {
  oled.set2X();
  oled.clear();

  int value;
  // Read scaled value from EEPROM
  if (indicator >= 5 && indicator <= 7) value = EEPROM.read(indicator + 19) * 10;
  else value = EEPROM.read(indicator + 19);

  int temp = -1, limit = 255;
  // Set specific limits for certain parameters
  if (indicator == 1) limit = 25;
  else if (indicator == 2) limit = 50;
  else if (indicator >= 5 && indicator <= 7) limit = 2500;

  while (1) {
    if (temp != value) {
      temp = value;
      // Display motor error with its offset
      if (indicator == 2) text("SET: " + String(value - 25), 5, 3);
      else text("SET: " + String(value), 5, 3);
    }

    byte enc = encoder();
    if (enc) {
      // Some values increment by 10
      int step = (indicator >= 5 && indicator <= 7) ? 10 : 1;
      if (enc == 1) { value += step; }
      else { value -= step; }
      // Constrain value within limits
      if (value > limit) value = limit;
      if (value < 0) value = 0;
    }

    byte r = push(SW_PIN);
    if (r) {
      if (r == 1) { // Short press: save to EEPROM
        byte valueToSave = (indicator >= 5 && indicator <= 7) ? value / 10 : value;
        EEPROM.write(19 + indicator, valueToSave);
        delay(10);
      }
      oled.set1X();
      return; // Exit on any press (saved or cancelled)
    }
  }
}


/**
 * @brief Returns the string for a given adjustment item.
 * @param a The adjustment item number.
 * @return The corresponding adjustment text.
 */
String adjustment_list(byte a) {
  if (a == 1) return "Top Speed";
  else if (a == 2) return "Motor Error";
  else if (a == 3) return "Turn Speed";
  else if (a == 4) return "Turn Brake";
  else if (a == 5) return "Brake Delay";
  else if (a == 6) return "Turn 90 Delay";
  else if (a == 7) return "U Turn Timer";
  else if (a == 8) return "Stop Timer";
  else if (a == 9) return "Node Delay";
  else if (a == 10) return "Obj Dist.";
  else if (a == 11) return "Wall Dist.";
  else if (a == 12) return "Wall Mid";
  else if (a == 13) return "Wall Prop.";
  else if (a == 14) return "Inv. Timer";
  return "INVALID";
}


// --- PATH & COUNTER PANELS ---

/**
 * @brief Displays the panel for viewing and selecting a path step to edit.
 */
void path_panel() {
  oled.set1X();
  oled.clear();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 25;
  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8 ; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        text(String(i + base) + ") " + path_list(path[i + base - 1]), 0, i);
      }
    }

    byte enc = encoder();
    if (enc) {
      if (enc == 1) { ind++; if (ind > limit) ind = 1; }
      else { ind--; if (ind < 1) ind = limit; }
      if (ind > peak) { peak = ind; base = peak - 7; }
      else if (ind < base) { base = ind; peak = base + 7; }
    }

    byte r = push(SW_PIN);
    if (r) {
      if (r == 1) path_adjust(ind - 1); // Edit selected path step
      else return; // Exit on long press
      temp = 0; // Force redraw
    }
  }
}

/**
 * @brief Allows editing a single step in the path array.
 * @param indicator The index of the path step to edit.
 */
void path_adjust(int indicator) {
  oled.set2X();
  oled.clear();
  int value = path[indicator];
  int temp = -1, limit = 16;
  while (1) {
    if (temp != value) {
      temp = value;
      text(path_list(value), 5, 3);
    }

    byte enc = encoder();
    if (enc) {
      if (enc == 1) { value++; }
      else { value--; }
      if (value > limit) value = limit;
      if (value < 0) value = 0;
    }

    byte r = push(SW_PIN);
    if (r) {
      oled.set1X();
      if (r == 1) { // Save to path array and EEPROM
        path[indicator] = value;
        EEPROM.write(memory + indicator, value);
        delay(10);
      }
      return;
    }
  }
}

/**
 * @brief Returns the string for a given path command.
 * @param a The path command number.
 * @return The corresponding command text.
 */
String path_list(byte a) {
  if (a == 0)  return "EMPTY";
  else if (a == 1) return "LEFT";
  else if (a == 2) return "STRAIGHT";
  else if (a == 3) return "RIGHT";
  else if (a == 4) return "T-LEFT";
  else if (a == 5) return "T-RIGHT";
  else if (a == 6) return "CROSS LEFT";
  else if (a == 7) return "CROSS STRAIGHT";
  else if (a == 8) return "CROSS RIGHT";
  else if (a == 9) return "90 DETECT";
  else if (a == 10) return "U TURN";
  else if (a == 11) return "LINE GAP";
  else if (a == 12) return "WALL FOLLOW";
  else if (a == 13) return "OBSTACLE";
  else if (a == 14) return "CONTINUE";
  else if (a == 15) return "INVERSE";
  else if (a == 16) return "CAPACITOR";
  else return "INVALID";
}

/**
 * @brief Allows adjusting the starting path counter.
 */
void counter_adjust() {
  oled.set2X();
  oled.clear();
  int temp_counter = EEPROM.read(19);
  byte temp = -1, limit = 25;
  while (1) {
    if (temp != temp_counter) {
      temp = temp_counter;
      text("SET: " + String(temp_counter), 5, 3);
    }

    byte enc = encoder();
    if (enc) {
      if (enc == 1) { temp_counter++; }
      else { temp_counter--; }
      if (temp_counter > limit) temp_counter = limit;
      if (temp_counter < 0) temp_counter = 0;
    }

    byte r = push(SW_PIN);
    if (r) {
      if (r == 1) { // Save to EEPROM
        EEPROM.write(19, temp_counter);
        delay(10);
        counter = temp_counter; // Update global counter
      }
      oled.set1X();
      return;
    }
  }
}


// --- DISPLAY & DEBUG SCREENS ---

/**
 * @brief Displays a splash screen.
 */
void TechTopia() {
  oled.clear();
  oled.set2X();
  oled.setInvertMode(0);
  text("  TECH  ", 4, 1);
  text(" TOPIA ", 10, 4);
}

/**
 * @brief Displays the live analog readings from IR sensors.
 * --- MODIFIED to use ir_pins[] array ---
 */
void analog_display() {
  oled.clear();
  oled.set2X();
  int temp_s[6];
  while (!push(SW_PIN)) {
    // Read from the pins defined in the ir_pins array
    for (int i = 0 ; i < 6 ; i++) temp_s[i] = analogRead(ir_pins[i]);
    
    for (byte i = 0; i < 3; i++) {
      text(String(temp_s[i+3]) + "    ", 0, i * 2);
      text(String(temp_s[2-i]) + "    ", 64, i * 2);
    }
  }
  oled.set1X();
}

/**
 * @brief Displays live digital readings from all sensors.
 */
void digital_display() {
  oled.clear();
  oled.set2X();
  text("L", 24, 4);
  text("F", 59, 4);
  text("R", 94, 4);
  while (!push(SW_PIN)) {
    reading();
    sonarf_read(true); // Force a reading
    sonarr_read(true);
    sonarl_read(true);
    for (byte i = 0; i < 6; i++)
      text(String(s[5 - i]), 1 + i * 23, 1);
    
    (sl < 10) ? text(" " + String(sl) + " ", 12, 6) : text(String(sl), 13, 6);
    (sf < 10) ? text(" " + String(sf) + " ", 47, 6) : text(String(sf), 48, 6);
    (sr < 10) ? text(" " + String(sr) + " ", 82, 6) : text(String(sr), 83, 6);
  }
  oled.set1X();
}

/**
 * @brief Displays the calibration screen and runs the calibration routine.
 */
void calibration_display() {
  oled.clear();
  oled.set2X();
  text(" STARTING ", 4, 2);
  oled.set1X();
  for (byte i = 0; i < 42; i++) {
    text(".", i * 3, 4);
    delay(25);
    if (i >= 41) cal();
    if (push(SW_PIN)) break;
  }

  oled.clear();
  text("MIN", 0, 0);
  text("MID", 0, 3);
  text("MAX", 0, 6);
  for (byte i = 0; i < 3; i++) {
    text(String(minimum[5 - i]), 30 + i * 37, 0);
    text(String(minimum[i]), 30 + i * 37, 1);
    text(String(threshold[5 - i]), 30 + i * 37, 3);
    text(String(threshold[i]), 30 + i * 37, 4);
    text(String(maximum[5 - i]), 30 + i * 37, 6);
    text(String(maximum[i]), 30 + i * 37, 7);
  }
  while (!push(SW_PIN));
}


/**
 * @brief Helper function to print text to the OLED.
 * @param t The string to print.
 * @param x The x-coordinate.
 * @param y The y-coordinate (row).
 */
void text(String t, byte x, byte y) {
  oled.setCursor(x, y);
  oled.print(t);
}
