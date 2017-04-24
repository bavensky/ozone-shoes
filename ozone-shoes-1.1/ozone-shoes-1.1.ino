/*
   Project  : Ozone shoes
   Hardware : Arduino UNO, LED, Buzzer, Relay
   Author   : Bavensky :3
   E-Mail   : apiruk326@gmail.com
   Date     : 19/04/2017 [dd/mm/yyyy]
*/

/*********   Include Library *********/
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CountUpDownTimer.h>

/*********   Initial Variable *********/
LiquidCrystal_I2C lcd(0x3f, 16, 2);
CountUpDownTimer T(DOWN);

#define B_START 9
#define B_SET   8
#define LED_COUNT 7
#define LED_STOP 6
#define RELAY_FAN 5
#define RELAY_OZONE 4
#define BUZZER 3
#define db 200

#define debounce 30
#define holdTime 800
int btnLast = 0;
boolean ignore = false;
unsigned long btnDownT;
unsigned long btnUpT;

int address = 9;
int timecount = 0;
int mode = 0;
int numLoop = 0;
int numState = 0;
boolean loopState = false;
boolean loopCountState = false;

void toneBuzzer(int up, int down) {
  digitalWrite(BUZZER, HIGH);
  delay(up);
  digitalWrite(BUZZER, LOW);
  delay(down);
}

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

/*********   Main Code *********/
void setup()  {
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
  delay(500);  //5000
  lcd.clear();
}

void loop() {
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
      digitalWrite(RELAY_FAN, HIGH);
      digitalWrite(RELAY_OZONE, HIGH);
      T.SetTimer(0, timecount, 0);
      T.StartTimer();
      mode = 3;
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
      mode = 1;
    }
    btnLast = digitalRead(B_SET);
  }




  // loop mode
  while (mode == 1) {
    selectMode();
  }
  while (mode == 2) {
    loopCount();
  }
  while (mode == 3) {
    timeCount();
  }
  while (mode == 4) {
    setTimes();
  }
  while (mode == 5) {
    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("    Complete    ");
    if (digitalRead(B_START) == 0)  {
      delay(db);
      digitalWrite(RELAY_FAN, HIGH);
      digitalWrite(RELAY_OZONE, HIGH);
      mode = 0;
    }
  }
}
