/**
 * @file Navigation.ino
 * @brief Contains the main line following logic and advanced navigation routines.
 * This includes obstacle avoidance and wall following, all dictated by the
 * pre-programmed path array.
 */

/**
 * @brief The main control loop for line following and executing path commands.
 */
void line_follow() {
  delay(500);
  counter = EEPROM.read(19); // Start from the counter value set in the menu
  
  while (1) {
    // Check for exit condition (long press)
    if (push(SW_PIN) == 2) {
        motor(0,0);
        return;
    }

    reading();

    // --- STATE 1: NO LINE DETECTED (sum == 0) ---
    if (sum == 0) {
      // If a turn was commanded ('l' or 'r'), execute it.
      if (turn != 's') {
        if (path[counter] == 9) { // 90-degree turn detection
          counter++;
          digitalWrite(LED_PIN, HIGH);
        }
        brake();
        (turn == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        while (!s[2] && !s[3]) reading(); // Wait until center sensors find the line
        (turn == 'r') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        delay(turn_brake);
        turn = 's'; cross = 's'; pos = 0;
      }
      // If no turn is commanded, check for programmed actions for line gaps.
      else if (pos > -3 && pos < 3) {
        // Check if the path commands wall following (12)
        if (path[counter] == 12 || (path[counter] == 14 && path[counter + 1] == 12))
          if (sonarl_read(false) || sonarr_read(false)) wall_follow();

        m2 = millis();
        while (!sum) {
          reading();
          // Timeout check for U-Turn (10)
          if (millis() - m2 > u_turn_timer) {
            turn = side;
            if (path[counter] == 10 || (path[counter] == 14 && path[counter + 1] == 10)) {
              (path[counter] == 14) ? counter += 2 : counter++;
              digitalWrite(LED_PIN, HIGH);
            }
            m1 = millis();
            break;
          }
        }
        // Check for simple Line Gap (11)
        if (path[counter] == 11 || (path[counter] == 14 && path[counter + 1] == 11)) {
          (path[counter] == 14) ? counter += 2 : counter++;
          digitalWrite(LED_PIN, HIGH);
          m1 = millis();
        }
      }
    }

    // --- STATE 2: ON THE LINE (sum == 1 or 2) ---
    else if (sum == 1 || sum == 2) {
      // Check for programmed Obstacle detection (13)
      if (path[counter] == 13 || (path[counter] == 14 && path[counter + 1] == 13))
        if (sonarf_read(false)) obstacle('l'); // Example: default to left obstacle avoidance

      // If a cross-turn was commanded, execute it.
      if (cross != 's') {
        brake();
        (cross == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        while (sum > 1) reading();
        while (!s[2] && !s[3]) reading();
        (cross == 'r') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        delay(turn_brake);
        cross = 's'; turn = 's'; pos = 0;
      }

      // Standard PID-like line following logic
      if (sensor == 0b001100) { // Center
        if (pos != 0) { // Apply correction if there was recent error
          (pos > 0) ? motor(-10 * spl, 10 * spr) : motor(10 * spl, -10 * spr);
          (pos > 0) ? delay(pos * 5) : delay(-pos * 5);
          pos = 0;
        }
        motor(10 * spl, 10 * spr);
      }
      // Right side deviations
      else if (sensor == 0b000100) motor(10 * spl, 9 * spr);
      else if (sensor == 0b000110) { if (pos < 1) pos = 1; motor(10 * spl, 6 * spr); }
      else if (sensor == 0b000010) { if (pos < 2) pos = 2; motor(10 * spl, 3 * spr); }
      else if (sensor == 0b000011) { if (pos < 3) pos = 3; motor(10 * spl, 0 * spr); }
      else if (sensor == 0b000001) { if (pos < 4) pos = 4; motor(10 * spl, -3 * spr); }
      // Left side deviations
      else if (sensor == 0b001000) motor(9 * spl, 10 * spr);
      else if (sensor == 0b011000) { if (pos > -1) pos = -1; motor(6 * spl, 10 * spr); }
      else if (sensor == 0b010000) { if (pos > -2) pos = -2; motor(3 * spl, 10 * spr); }
      else if (sensor == 0b110000) { if (pos > -3) pos = -3; motor(0 * spl, 10 * spr); }
      else if (sensor == 0b100000) { if (pos > -4) pos = -4; motor(-3 * spl, 10 * spr); }
    }

    // --- STATE 3: INTERSECTION (sum >= 3) ---
    else if (sum >= 3) {
      // Check for programmed Inverse Line detection (15)
      if (path[counter] == 15 || (path[counter] == 14 && path[counter + 1] == 15)) i_detection();

      // Left turn detected
      if (s[5] && !s[0]) {
        turn = 'l';
        // Check if path allows a left turn (1), straight (2), or is a continue node (14)
        if (path[counter] == 1 || path[counter] == 2 || path[counter] == 14) {
          while (s[5] && !s[0]) reading();
          if (s[0] == 0) {
            delay(node_delay);
            reading();
            if (sum) { // If still on a line after the node
              if (path[counter] == 1) cross = side = 'l';
              else if (path[counter] == 14) cross = side;
              if (path[counter] != 14) {
                counter++;
                digitalWrite(LED_PIN, HIGH);
              }
            }
          }
        }
      }
      // Right turn detected
      else if (!s[5] && s[0]) {
        turn = 'r';
        // Check if path allows a right turn (3), straight (2), or is a continue node (14)
        if (path[counter] == 3 || path[counter] == 2 || path[counter] == 14) {
          while (!s[5] && s[0]) reading();
          if (s[0] == 0) {
            delay(node_delay);
            reading();
            if (sum) {
              if (path[counter] == 3) cross = side = 'r';
              else if (path[counter] == 14) cross = side;
              if (path[counter] != 14) {
                counter++;
                digitalWrite(LED_PIN, HIGH);
              }
            }
          }
        }
      }
      m1 = millis();
    }

    // --- STATE 4: FULL LINE / END MARKER (sum == 6) ---
    else if (sum == 6) {
      turn = side;
      m2 = millis();
      while (s[5] || s[0]) {
        reading();
        // Stop timer check
        if (millis() - m2 > stop_timer) {
          motor(0, 0);
          return; // End of run
        }
      }
      delay(node_delay);
      reading();
      if (sum) {
        // Cross-junction logic (6, 7, 8)
        if (path[counter] >= 6 && path[counter] <= 8) {
          if (path[counter] == 6) cross = side = 'l';
          else if (path[counter] == 8) cross = side = 'r';
          counter++;
          digitalWrite(LED_PIN, 1);
        } else if (path[counter] == 14) cross = side;
      }
      // T-junction logic (4, 5)
      else if (path[counter] == 4 || path[counter] == 5) {
        (path[counter] == 4) ? turn = side = 'l' : turn = side = 'r';
        counter++;
        digitalWrite(LED_PIN, 1);
      }
      // Capacitor charge point logic (16)
      else if (path[counter] == 16) {
        turn = 's';
        while (!s[5] || !s[0]) reading();
        while (s[5] || s[0]) reading();
        delay(node_delay);
        counter++;
        digitalWrite(LED_PIN, 1);
      }
      m1 = millis();
    }

    // Reset turn flags and LED after a short delay
    if (millis() - m1 > 500) {
      turn = 's';
      digitalWrite(LED_PIN, LOW);
    }
  }
}

/**
 * @brief Navigates a gap in the line by following a wall.
 */
void wall_follow() {
  while (!sum) { // Continue until a line is found
    // Check for exit condition
    if (push(SW_PIN) == 2) {
        motor(0,0);
        return;
    }
    reading();
    // Follow right wall if detected
    while (sonarr_read(true)) {
      int wall_error = (wall_mid - sr) * wallp;
      motor(150 + wall_error, 150 - wall_error);
    }
    // Follow left wall if detected
    while (sonarl_read(true)) {
      int wall_error = (wall_mid - sl) * wallp;
      motor(150 - wall_error, 150 + wall_error);
    }
    motor(150, 150); // Go straight if no wall
  }

  // Line found, advance path counter
  (path[counter] == 14) ? counter += 2 : counter++;
  digitalWrite(LED_PIN, HIGH);
  m1 = millis();
}

/**
 * @brief Avoids an obstacle detected by the front sensor.
 * @param s The direction to turn ('l' for left, 'r' for right).
 */
void obstacle(char s) {
  brake();
  turn_90(s);
  reading();
  // Move forward at an angle to clear the obstacle
  (s == 'l') ? motor(10 * spl, 5 * spr) : motor(5 * spl, 10 * spr);
  while (!sum) reading(); // Wait until a line is found
  while (sum) reading();  // Wait until past the line
  delay(node_delay);
  motor(0, 0);
  delay(100);
  turn_90(s); // Turn back to align with the original path direction

  // Advance path counter
  (path[counter] == 14) ? counter += 2 : counter++;
  digitalWrite(LED_PIN, HIGH);
  m1 = millis();
}
