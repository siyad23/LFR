String path_list(byte a) {
  if (a == 0)      return "EMPTY               ";
  else if (a == 1) return "LEFT                ";
  else if (a == 2) return "STRAIGHT            ";
  else if (a == 3) return "RIGHT               ";
  else if (a == 4) return "T-LEFT              ";
  else if (a == 5) return "T-RIGHT             ";
  else if (a == 6) return "CROSS LEFT          ";
  else if (a == 7) return "CROSS STRAIGHT      ";
  else if (a == 8) return "CROSS RIGHT         ";
  else if (a == 9) return "90 DETECT           ";
  else if (a == 10) return "U TURN              ";
  else if (a == 11) return "LINE GAP            ";
  else if (a == 12) return "WALL FOLLOW         ";
  else if (a == 13) return "OBSTACLE            ";
  else if (a == 14) return "CONTINUE            ";
  else if (a == 15) return "INVERSE             ";
  else if (a == 16) return "CAPACITOR           ";
  else              return "INVALID             ";
}

void path_panel() {
  oled.set1X();
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
      if (enc == 1) {
        ind++;
        if (ind > limit) {
          ind = 1;
          base = ind;
          peak = base + 7;
        }
        else if (ind > peak) {
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
        }
        else if (ind < base) {
          base = ind;
          peak = base + 7;
        }
      }
    }

    byte r = push(sw);
    if (r) {
      if (r == 1) path_adjust(ind - 1);
      else {
        eeprom_refresh();
        return;
      }
      temp = 0;
    }
  }
}

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
      if (enc == 1) {
        value++;
        if (value > limit) value = limit;
      } else {
        value--;
        if (value < 0) value = 0;
      }
    }

    byte r = push(sw);
    if (r) {
      oled.set1X();
      if (r == 1) {
        path[indicator] = value;
        EEPROM.write(memory + indicator, value);
        delay(10);
        return;
      } else return;
    }
  }
}

