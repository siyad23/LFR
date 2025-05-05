String menu_list(byte a) {
  if (a == 1)       return "Line Follow       ";
  else if (a == 2)  return "Counter           ";
  else if (a == 3)  return "Adjustment        ";
  else if (a == 4)  return "Path Adjust       ";
  else if (a == 5)  return "Calibration       ";
  else if (a == 6)  return "Analog Display    ";
  else if (a == 7)  return "Digital Display   ";
  else if (a == 8)  return "Go Forward        ";
  else if (a == 9)  return "Rotation Test     ";
  else if (a == 10)  return "Path Clear       ";
  else if (a == 11)  return "Memory Clear     ";
}

byte menu() {
  oled.set1X();
  byte base = 1, peak = 8, temp = 0, ind = 1, limit = 11;
  bool u_flag = 0, d_flag = 0;
  while (1) {
    if (temp != ind) {
      temp = ind;
      for (byte i = 0; i < 8 ; i++) {
        (i == ind - base) ? oled.setInvertMode(1) : oled.setInvertMode(0);
        text(String(i + base) + ") " + menu_list(i + base), 0, i);
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
      if (r == 1) return ind;
      else return 0;
    }
  }
}
