void TechTopia() {
  oled.clear();
  oled.set2X();
  oled.setInvertMode(0);
  text("   TECH   ", 4, 1);
  text("  TOPIA  ", 10, 4);
}

void text(String t, byte x, byte y) {
  oled.setCursor(x, y);
  oled.print(t);
}
