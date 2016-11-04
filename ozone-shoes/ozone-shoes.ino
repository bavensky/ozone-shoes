/*
   Project  : Ozone shoes
   Hardware : Arduino UNO, LED, Buzzer, Relay
   Author   : Bavensky :3
   E-Mail   : apiruk326@gmail.com
   Date     : 04/11/2016 [dd/mm/yyyy]
*/

/*********   Include Library *********/
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<CountUpDownTimer.h>

/*********   Initial Variable *********/
LiquidCrystal_I2C lcd(0x3f, 16, 2);
CountUpDownTimer T(DOWN);

#define B_START 8
#define B_SET   9
#define LED_COUNT 7
#define LED_STOP 6
#define RELAY_FAN 5
#define RELAY_OZONE 4
#define BUZZER 3
#define db 200

#define debounce 50
#define holdTime 1000
int btnLast = 0;
boolean ignore = false;
unsigned long btnDownT;
unsigned long btnUpT;

int address = 9;
int timecount = 0;
int mode = 0;

/*********   Sub Function *********/
void timeCount()  {
  digitalWrite(LED_COUNT, HIGH);
  digitalWrite(LED_STOP, LOW);

  T.Timer();
  if (T.ShowMinutes() > 0)  {
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_OZONE, LOW);
//    lcd.setCursor(0, 1);
//    lcd.print("Ozone ");
  } else if (T.ShowMinutes() <= 0)  {
    digitalWrite(RELAY_FAN, LOW);
    digitalWrite(RELAY_OZONE, HIGH);
//    lcd.setCursor(0, 1);
//    lcd.print("Fan   ");
  }
  
  if(T.ShowMinutes() == 1)  {
    toneBuzzer(50, 50);
    toneBuzzer(50, 50);
  }
  
  if (digitalRead(B_START) == 0)  {
    delay(db);
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_OZONE, HIGH);
    mode = 0;
  }

  if (T.ShowMinutes() <= 0 && T.ShowSeconds() <= 0)  {
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_OZONE, HIGH);
    digitalWrite(LED_COUNT, LOW);
    digitalWrite(LED_STOP, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("    Complete    ");
    toneBuzzer(1000, 0);
    toneBuzzer(100, 50);
    toneBuzzer(100, 50);
    mode = 0;
  }

  lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
  //  lcd.setCursor(6, 1);
  lcd.setCursor(0, 1);
  lcd.print("Count ");
  if (T.ShowMinutes() < 10)  lcd.print("0");
  lcd.print(T.ShowMinutes());
  lcd.print(":");
  if (T.ShowSeconds() < 10)  lcd.print("0");
  lcd.print(T.ShowSeconds());
  lcd.print(" m/s   ");
}

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
  lcd.print("   Ozone shoes  ");
  lcd.setCursor(0, 1);
  lcd.print("  Set ");
  if (timecount < 10) lcd.print("0");
  lcd.print(timecount);
  lcd.print(":00 m/s    ");
}

void toneBuzzer(int up, int down) {
  digitalWrite(BUZZER, HIGH);
  delay(up);
  digitalWrite(BUZZER, LOW);
  delay(down);
}

/*********   Main Code *********/
void setup()
{
  Serial.begin(115200);

  pinMode(B_START, INPUT_PULLUP);
  pinMode(B_SET, INPUT_PULLUP);
  pinMode(LED_COUNT, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_OZONE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_COUNT, LOW);
  digitalWrite(LED_STOP, LOW);
  digitalWrite(RELAY_FAN, HIGH);
  digitalWrite(RELAY_OZONE, HIGH);
  digitalWrite(BUZZER, LOW);

  timecount = EEPROM.read(address);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("     Welcome    ");
  lcd.setCursor(0, 1);
  lcd.print("   Ozone shoes  ");
  delay(500); // 5000
  lcd.clear();
}

void loop()
{
  while (mode == 0) {
    digitalWrite(LED_COUNT, LOW);
    digitalWrite(LED_STOP, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("Time  ");
    if (timecount < 10) lcd.print("0");
    lcd.print(timecount);
    lcd.print(":00 m/s  ");

    if (digitalRead(B_START) == 0)  {
      delay(db);
      toneBuzzer(1000, 0);
      T.SetTimer(0, timecount, 0);
      T.StartTimer();
      mode = 1;
    }

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
      toneBuzzer(50, 50);
      toneBuzzer(50, 50);
      toneBuzzer(50, 50);
      toneBuzzer(100, 0);
      lcd.clear();
      mode = 2;
    }
    btnLast = digitalRead(B_SET);

  }
  while (mode == 1) {
    timeCount();
  }
  while (mode == 2) {
    setTimes();
  }
}
