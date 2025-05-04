byte encoder() {
  bool newClk = digitalRead(ENCODER_CLK);
  if (newClk != lastClk) {
    lastClk = newClk;
    bool dtValue = digitalRead(ENCODER_DT);
    if (newClk == LOW && dtValue == HIGH) return 1;
    if (newClk == LOW && dtValue == LOW) return 2;
  }
  return 0;
}

byte push(byte button_number) {
  uint32_t b_timer = 0;
  if (!digitalRead(button_number)) {  //if button is pressed
    delay(50);                        //debounce delay;
    while (!digitalRead(button_number)) {
      delay(50);
      b_timer += 50;
      if (b_timer > 1000) digitalWrite(led, 1);
    }
    digitalWrite(led, 0);
    if (b_timer > 1000) return 2;    //long press
    else if (b_timer > 0) return 1;  //short press
  }
  return 0;
}
