void i_detection() {
  if (s[5] && s[0]) {  //inverse line detection
    uint32_t timer = millis();
    while ((s[5] || s[0]) && sum < 6) {
      reading();
      if (millis() - timer > i_timer) {
        i_mode = !i_mode;
        digitalWrite(led, i_mode);
        cross = 's';
        if (!i_mode) {
          (path[counter] == 14) ? counter += 2 : counter++;
          digitalWrite(led, HIGH);
          m1 = millis();
        }
      }
    }
    turn = side;  //specific guided turn for Y section
  }
  counter++;
}
