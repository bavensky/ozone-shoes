void selectMode()  {
  lcd.setCursor(0, 0);
  lcd.print("Select Mode :   ");
  lcd.setCursor(0, 1);
  lcd.print("Mode1  ||  Mode2");
  delay(500);
  
  if (digitalRead(B_START) == 0)  {
    delay(db);
    lcd.setCursor(0, 0);
    lcd.print("Select Mode :   ");
    lcd.setCursor(0, 1);
    lcd.print("Mode1 Loop count");
    delay(2000);
    lcd.clear();
    mode = 2; // loopcount
  }
  if (digitalRead(B_SET) == 0)  {
    delay(db);
    lcd.setCursor(0, 0);
    lcd.print("Select Mode :   ");
    lcd.setCursor(0, 1);
    lcd.print("Mode2 Time count");
    delay(2000);
    lcd.clear();
    ignore = false;
    mode = 4; // setTimes
  }
}

