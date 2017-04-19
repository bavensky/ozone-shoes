void setTimes()  {
  if (digitalRead(B_START) == LOW)  {
    delay(db);
    toneBuzzer(50, 50);
    timecount += 1;
  }
  if (digitalRead(B_SET) == LOW)  {
    delay(db);
    if (digitalRead(B_SET) == HIGH && ignore == false)  {
      toneBuzzer(50, 50);
      timecount -= 1;
    }
  }

  if (timecount <= 2) timecount = 2;

  if (digitalRead(B_SET) == LOW && btnLast == HIGH && (millis() - btnUpT) > long(debounce)) {
    btnDownT = millis();
  }
  if (digitalRead(B_SET) == HIGH && btnLast == LOW && (millis() - btnDownT) > long(debounce)) {
    ignore = false;
    btnUpT = millis();
  }
  if (digitalRead(B_SET) == LOW && (millis() - btnDownT) > long(holdTime)) {
    ignore = true;
    btnDownT = millis();
    EEPROM.write(address, timecount);
    toneBuzzer(50, 50);
    toneBuzzer(50, 50);
    toneBuzzer(50, 50);
    toneBuzzer(100, 0);
    mode = 0;
  }
  btnLast = digitalRead(B_SET);

  lcd.setCursor(0, 0);
  lcd.print("Mode2 Time count");
  lcd.setCursor(0, 1);
  lcd.print("  Set ");
  if (timecount < 10) lcd.print("0");
  lcd.print(timecount);
  lcd.print(":00 m/s    ");
}
