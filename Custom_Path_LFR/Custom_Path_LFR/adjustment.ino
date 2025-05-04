String adjustment_list(byte a) {
  if (a == 1) return "Top Speed     ";
  else if (a == 2) return "Motor Error   ";
  else if (a == 3) return "Turn Speed    ";
  else if (a == 4) return "Turn Brake    ";
  else if (a == 5) return "Brake Delay   ";
  else if (a == 6) return "Turn 90 Delay ";
  else if (a == 7) return "U Turn Timer  ";
  else if (a == 8) return "Stop Timer    ";
  else if (a == 9) return "Node Delay    ";
  else if (a == 10) return "Obj Dist.    ";
  else if (a == 11) return "Wall Dist.   ";
  else if (a == 12) return "Wall Mid     ";
  else if (a == 13) return "Wall Prop.   ";
  else if (a == 14) return "Inv. Timer   ";
}

void adjustment_panel() {
  oled.set1X();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 14;
  int value;
  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        if (i + base >= 5 && i + base <= 7) value = EEPROM.read(i + base + 19) * 10;
        else value = EEPROM.read(i + base + 19);
        if (i + base == 2) text(String(i + base) + ") " + adjustment_list(i + base) + String(value - 25) + " ", 0, i);
        else text(String(i + base) + ") " + adjustment_list(i + base) + String(value) + "   ", 0, i);
      }
    }

    byte enc = encoder();
    if (enc) {
      if (enc == 1) {
        ind++;
        if (ind > limit) {
          ind = 1;
          base = ind;
          peak = base + 7;
        } else if (ind > peak) {
          peak = ind;
          base = peak - 7;
        }
      }

      else {
        ind--;
        if (ind < 1) {
          ind = limit;
          peak = ind;
          base = peak - 7;
        } else if (ind < base) {
          base = ind;
          peak = base + 7;
        }
      }
    }

    byte r = push(sw);
    if (r) {
      if (r == 1) set_adjust(ind);
      else {
        eeprom_refresh();
        return;
      }
      temp = 0;
    }
  }
}

void set_adjust(int indicator) {
  oled.set2X();
  oled.clear();

  int value;
  if (indicator >= 5 && indicator <= 7) value = EEPROM.read(indicator + 19) * 10;
  else value = EEPROM.read(indicator + 19);

  int temp = -1, limit = 255;

  if (indicator == 1) limit = 25;
  else if (indicator == 2) limit = 50;
  else if (indicator >= 5 && indicator <= 7) limit = 2500;

  while (1) {
    if (temp != value) {
      temp = value;
      if (indicator == 2) text("SET: " + String(value - 25) + "    ", 5, 3);
      else text("SET: " + String(value) + "    ", 5, 3);
    }

    byte enc = encoder();
    if (enc) {
      if (enc == 1) {
        (indicator >= 5 && indicator <= 7) ? value += 10 : value++;
        if (value > limit) value = limit;
      }

      else {
        (indicator >= 5 && indicator <= 7) ? value -= 10 : value--;
        if (value < 0) value = 0;
      }
    }

    byte r = push(sw);
    if (r) {
      if (r == 1) {
        (indicator >= 5 && indicator <= 7) ? EEPROM.write(19 + indicator, value / 10) : EEPROM.write(19 + indicator, value);
        delay(10);
        oled.set1X();
        return;
      } else return;
    }
  }
}

void counter_adjust() {
  oled.set2X();
  oled.clear();
  counter = EEPROM.read(19);
  byte temp = -1, limit = 25;
  while (1) {
    if (temp != counter) {
      temp = counter;
      text("SET: " + String(counter) + "    ", 5, 3);
    }

    byte enc = encoder();
    if (enc) {
      if (enc == 1) {
        counter++;
        if (counter > limit) counter = limit;
      } else {
        counter--;
        if (counter < 0) counter = 0;
      }
    }

    byte r = push(sw);
    if (r) {
      if (r == 1) {
        EEPROM.write(19, counter);
        delay(10);
        oled.set1X();
        return;
      } else {
        counter = EEPROM.read(19);
        return;
      }
    }
  }
}
