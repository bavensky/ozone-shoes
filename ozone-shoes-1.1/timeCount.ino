void timeCount()  {
  T.Timer();

  lcd.setCursor(0, 0);
  lcd.print("   Baked Ozone  ");
  lcd.setCursor(0, 1);
  lcd.print("Count ");
  if (T.ShowMinutes() < 10)  lcd.print("0");
  lcd.print(T.ShowMinutes());
  lcd.print(":");
  if (T.ShowSeconds() < 10)  lcd.print("0");
  lcd.print(T.ShowSeconds());
  lcd.print(" m/s   ");

  if (digitalRead(B_START) == 0)  {
    delay(db);
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_OZONE, HIGH);
    mode = 0;
  }

  if (T.ShowMinutes() <= 0 && T.ShowSeconds() <= 0)  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("   Drain Ozone  ");
    digitalWrite(RELAY_FAN, LOW);
    delay(10000);
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(LED_COUNT, LOW);
    digitalWrite(LED_STOP, HIGH);

    toneBuzzer(500, 100);
    toneBuzzer(100, 100);
    toneBuzzer(100, 100);
    mode = 5;
  }
}
