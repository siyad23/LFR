void reading() {
  sensor = 0;
  sum = 0;
  for (byte i = 0; i < 6; i++) {
    s[i] = analogRead(i);
    (s[i] > threshold) ? s[i] = 1 ^ i_mode : s[i] = 0 ^ i_mode;
    sensor += s[i] * base[i];
    sum += s[i];
  }
}

void analog_display() {
  oled.clear();
  oled.set2X();
  while (!push(sw)) {
    for (int i = 0 ; i < 6 ; i++)
      (i < 4) ? s[i] = analogRead(i) : s[i] = analogRead(i + 2);
    for (byte i = 0; i < 3; i++) {
      if (s[i + 3] < 10) text(String(s[i + 3]) + "   ", 10, i * 3);
      else if (s[i + 3] < 100) text(String(s[i + 3]) + "  ", 10, i * 3);
      else if (s[i + 3] < 1000) text(String(s[i + 3]) + " ", 10, i * 3);
      else text(String(s[i + 3]), 10, i * 3);
      text(String(s[2 - i]) + "   ", 70, i * 3);
    }
  }
  oled.set1X();
}

void digital_display() {
  oled.clear();
  oled.set2X();
  text("L", 24, 4);
  text("F", 59, 4);
  text("R", 94, 4);
  while (!push(sw)) {
    reading();
    sonarf_read(0);
    sonarr_read(0);
    sonarl_read(0);
    for (byte i = 0; i < 6; i++)
      text(String(s[5 - i]), 1 + i * 23, 1);
    (sl < 10) ? text(" " + String(sl) + " ", 12, 6) : text(String(sl), 13, 6);
    (sf < 10) ? text(" " + String(sf) + " ", 47, 6) : text(String(sf), 48, 6);
    (sr < 10) ? text(" " + String(sr) + " ", 82, 6) : text(String(sr), 83, 6);
  }
  oled.set1X();
}

void calibration_display() {
  oled.clear();
  oled.set2X();
  text(" STARTING ", 04, 2);
  oled.set1X();
  for (byte i = 0; i < 42; i++) {
    text(".", i * 3, 4);
    delay(25);
    if (i >= 41) cal();
    if(push(sw)) break;
  }
  
  oled.clear();
  text("MIN",0,0);
  text("MID",0,3);
  text("MAX",0,6);
  for(byte i = 0; i<3; i++){
    text(String(minimum[5-i]),30+i*37,0);
    text(String(minimum[i]),30+i*37,1);
    text(String(threshold[5-i]),30+i*37,3);
    text(String(threshold[i]),30+i*37,4);
    text(String(maximum[5-i]),30+i*37,6);
    text(String(maximum[i]),30+i*37,7);
  }
  while(!push(sw));
}

void cal() {
  byte i = 0;
  for (i = 0; i < 6; i++) {
    maximum[i] = 0; minimum[i] = 1024;
  }
  // motor(-100, 100);
  for (short int c = 0; c < 3000 ; c++)
    for (i = 0; i < 6; i++) {
      (i < 4) ? s[i] = analogRead(i) : s[i] = analogRead(i + 2);
      maximum[i] = max(maximum[i], s[i]);
      minimum[i] = min(minimum[i], s[i]);
    }
  // motor(0, 0);
  for (i = 0; i < 6; i++) {
    threshold[i] = (maximum[i] - minimum[i]) * 0.5 + minimum[i];
    EEPROM.write(i, threshold[i] / 4 ); delay(10);
    EEPROM.write(i + 6, maximum[i] / 4 ); delay(10);
    EEPROM.write(i + 12, minimum[i] / 4 ); delay(10);
  }
  return;
}