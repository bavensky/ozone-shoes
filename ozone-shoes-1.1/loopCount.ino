void loopCount()  {
  lcd.setCursor(0, 0);
  lcd.print("Mode1 Loop count");
  lcd.setCursor(0, 1);
  lcd.print("Set loop = ");
  lcd.print(numLoop);
  lcd.print("   ");


  if (digitalRead(B_START) == LOW)  {
    delay(db);
    toneBuzzer(50, 50);
    numLoop += 1;
  }

  if (digitalRead(B_SET) == LOW)  {
    delay(db);
    if (digitalRead(B_SET) == HIGH && ignore == false)  {
      toneBuzzer(50, 50);
      numLoop -= 1;
    }
  }

  if (numLoop <= 0) numLoop = 0;

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
    loopCountState = true;
  }
  btnLast = digitalRead(B_SET);


  if (loopCountState == true) {
    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("  Start ");
    lcd.print(numLoop);
    lcd.print(" loop  ");
    delay(2000);

    for (int i = 1; i <= numLoop; i++) {
      Serial.print("Blowing ");
      Serial.println(timecount);
      lcd.setCursor(0, 0);
      lcd.print("   Ozone shoes  ");
      lcd.setCursor(0, 1);
      lcd.print("  Blowing Ozone ");

      // turn on fan and ozone machine
      digitalWrite(LED_COUNT, HIGH);
      digitalWrite(LED_STOP, LOW);
      digitalWrite(RELAY_FAN, LOW);
      digitalWrite(RELAY_OZONE, LOW);
      delay(10000);

      T.SetTimer(0, timecount, 0);
      loopState = true;

      while (loopState == true) {
        T.StartTimer();
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
          loopState = false;
        }
      }
    }
    loopCountState = false;
    mode = 5;
  }
}
