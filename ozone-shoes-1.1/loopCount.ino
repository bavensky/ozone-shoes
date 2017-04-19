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
    lcd.print("Start ");
    lcd.print(numLoop);
    lcd.print(" loop  ");
    delay(2000);

    for (int i = 1; i <= numLoop; i++) {
      toneBuzzer(1000, 0);

      lcd.setCursor(0, 0);
      lcd.print("   Ozone shoes  ");
      lcd.setCursor(0, 1);
      lcd.print("  Blowing Ozone ");
      digitalWrite(LED_COUNT, HIGH);
      digitalWrite(LED_STOP, LOW);
      digitalWrite(RELAY_FAN, LOW);
      digitalWrite(RELAY_OZONE, LOW);
      delay(10000);

      lcd.setCursor(0, 0);
      lcd.print("   Ozone shoes  ");
      lcd.setCursor(0, 1);
      lcd.print("   Drain Ozone  ");
      digitalWrite(RELAY_FAN, LOW);
      digitalWrite(RELAY_OZONE, HIGH);
      delay(10000);

      lcd.print("   Ozone shoes  ");
      lcd.setCursor(0, 1);
      lcd.print("    Complete    ");
      digitalWrite(RELAY_FAN, HIGH);
      digitalWrite(RELAY_OZONE, HIGH);
      digitalWrite(LED_COUNT, LOW);
      digitalWrite(LED_STOP, HIGH);

      toneBuzzer(500, 100);
      toneBuzzer(100, 100);
      toneBuzzer(100, 100);
      lcd.clear();
    }

    loopCountState = false;
    mode = 0;
  }
}

