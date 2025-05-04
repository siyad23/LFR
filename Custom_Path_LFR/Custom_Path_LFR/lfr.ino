void line_follow() {
  delay(500);
  counter = 0;
  while (1) {  //infinite loop
    reading();
    if (sum == 0) {
      if (turn != 's') {
        if (path[counter] == 9) {
          counter++;
          digitalWrite(led, HIGH);
        }
        brake();
        (turn == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        while (!s[2] && !s[3]) reading();
        (turn == 'r') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        delay(turn_brake);
        turn = 's';
        cross = 's';
        pos = 0;
      }

      else if (pos > -3 && pos < 3) {
        if (path[counter] == 12 || (path[counter] == 14 && path[counter + 1] == 12))
          if (sonarl_read(0) || sonarr_read(0)) wall_follow();
        m2 = millis();
        while (!sum) {
          reading();
          if (millis() - m2 > u_turn_timer) {
            turn = side;
            if (path[counter] == 10 || (path[counter] == 14 && path[counter + 1] == 10)) {
              (path[counter] == 14) ? counter += 2 : counter++;
              digitalWrite(led, HIGH);
            }
            m1 = millis();
            break;
          }
        }
        if (path[counter] == 11 || (path[counter] == 14 && path[counter + 1] == 11)) {
          (path[counter] == 14) ? counter += 2 : counter++;
          digitalWrite(led, HIGH);
          m1 = millis();
        }
      }
    }

    else if (sum == 1 || sum == 2) {  //only for straight line
      if (path[counter] == 13 || (path[counter] == 14 && path[counter + 1] == 13))
        if (sonarf_read(0)) obstacle('l');
      if (cross != 's') {
        brake();
        (cross == 'l') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        while (sum > 1) reading();
        while (!s[2] && !s[3]) reading();
        (cross == 'r') ? motor(-turn_speed, turn_speed) : motor(turn_speed, -turn_speed);
        delay(turn_brake);
        cross = 's';
        turn = 's';
        pos = 0;
      }

      if (sensor == 0b001100) {
        if (pos != 0) {
          (pos > 0) ? motor(-10 * spl, 10 * spr) : motor(10 * spl, -10 * spr);
          (pos > 0) ? delay(pos * 5) : delay(-pos * 5);
          pos = 0;
        }
        motor(10 * spl, 10 * spr);
      }
      //right side portion
      else if (sensor == 0b000100)
        motor(10 * spl, 9 * spr);

      else if (sensor == 0b000110) {
        if (pos < 1) pos = 1;
        motor(10 * spl, 6 * spr);
      }

      else if (sensor == 0b000010) {
        if (pos < 2) pos = 2;
        motor(10 * spl, 3 * spr);
      }

      else if (sensor == 0b000011) {
        if (pos < 3) pos = 3;
        motor(10 * spl, 0 * spr);
      }

      else if (sensor == 0b000001) {
        if (pos < 4) pos = 4;
        motor(10 * spl, -3 * spr);
      }

      //left side portion
      else if (sensor == 0b001000)
        motor(9 * spl, 10 * spr);
      else if (sensor == 0b011000) {
        if (pos > -1) pos = -1;
        motor(6 * spl, 10 * spr);
      }

      else if (sensor == 0b010000) {
        if (pos > -2) pos = -2;
        motor(3 * spl, 10 * spr);
      }

      else if (sensor == 0b110000) {
        if (pos > -3) pos = -3;
        motor(0 * spl, 10 * spr);
      }

      else if (sensor == 0b100000) {
        if (pos > -4) pos = -4;
        motor(-3 * spl, 10 * spr);
      }
    }

    else if (sum == 3 || sum == 4 || sum == 5) {
      if (path[counter] == 15 || (path[counter] == 14 && path[counter + 1] == 15)) i_detection();

      if (s[5] && !s[0]) {
        turn = 'l';
        if (path[counter] == 1 || path[counter] == 2 || path[counter] == 14) {
          while (s[5] && !s[0]) reading();
          if (s[0] == 0) {
            delay(node_delay);
            reading();
            if (sum) {
              if (path[counter] == 1) cross = side = 'l';
              else if (path[counter] == 14) cross = side;
              if (path[counter] != 14) {
                counter++;
                digitalWrite(led, HIGH);
              }
            }
          }
        }
      }

      else if (!s[5] && s[0]) {
        turn = 'r';
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
                digitalWrite(led, HIGH);
              }
            }
          }
        }
      }
      m1 = millis();
    }

    else if (sum == 6) {
      turn = side;
      m2 = millis();
      while (s[5] || s[0]) {
        reading();
        if (millis() - m2 > stop_timer) {
          motor(0, 0);
          return;
        }
      }
      delay(node_delay);
      reading();
      if (sum) {
        if (path[counter] >= 6 && path[counter] <= 8) {
          if (path[counter] == 6) cross = side = 'l';
          else if (path[counter] == 8) cross = side = 'r';
          counter++;
          digitalWrite(led, 1);
        } else if (path[counter] == 14) cross = side;
      }

      else if (path[counter] == 4 || path[counter] == 5) {
        (path[counter] == 4) ? turn = side = 'l' : turn = side = 'r';
        counter++;
        digitalWrite(led, 1);
      }
      else if(path[counter] == 16){
        turn = 's';
        while (!s[5] || !s[0]) reading();
        while (s[5] || s[0]) reading();
        delay(node_delay);
        counter++;
        digitalWrite(led, 1);
      }
      m1 = millis();
    }

    if (millis() - m1 > 500) {
      turn = 's';
      digitalWrite(led, LOW);
    }
  }
}
