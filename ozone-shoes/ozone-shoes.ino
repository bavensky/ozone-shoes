#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<CountUpDownTimer.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);
CountUpDownTimer T(DOWN);

#define B_START 8
#define B_SET   9
#define LED_COUNT 7
#define LED_STOP 6
#define RELAY_FAN 5
#define RELAY_OZONE 4
#define db 200

int address = 9;
int timecount = 5;
int mode = 0;

void timeCount()  {
  digitalWrite(LED_COUNT, HIGH);
  digitalWrite(LED_STOP, LOW);

  T.Timer();
  if (T.ShowMinutes() > 0)  {
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_OZONE, LOW);
  } else if (T.ShowMinutes() <= 0)  {
    digitalWrite(RELAY_FAN, LOW);
    digitalWrite(RELAY_OZONE, HIGH);
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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("    Time out    ");
    delay(2000);
    mode = 0;
  }

  lcd.setCursor(0, 0);
  lcd.print("   Ozone shoes  ");
  lcd.setCursor(0, 1);
  lcd.print("count : ");
  if (T.ShowMinutes() < 10)  lcd.print("0");
  lcd.print(T.ShowMinutes());
  lcd.print(":");
  if (T.ShowSeconds() < 10)  lcd.print("0");
  lcd.print(T.ShowSeconds());
  lcd.print(" s   ");
}

void setTimes()  {
  T.SetTimer(0, timecount, 0);
  T.StartTimer();
}

void setup()
{
  Serial.begin(115200);

  pinMode(B_START, INPUT_PULLUP);
  pinMode(B_SET, INPUT_PULLUP);
  pinMode(LED_COUNT, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_OZONE, OUTPUT);

  digitalWrite(LED_COUNT, LOW);
  digitalWrite(LED_STOP, LOW);
  digitalWrite(RELAY_FAN, HIGH);
  digitalWrite(RELAY_OZONE, HIGH);

  //   EEPROM.write(addr, val);
  //  timecount = EEPROM.read(address);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("     Welcome    ");
  lcd.setCursor(0, 1);
  lcd.print("   Ozone shoes  ");
  delay(2000);
  lcd.clear();

  T.SetTimer(0, timecount, 0);
  T.StartTimer();
}

void loop()
{
  while (mode == 0) {
    digitalWrite(LED_COUNT, LOW);
    digitalWrite(LED_STOP, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("   Ozone shoes  ");
    lcd.setCursor(0, 1);
    lcd.print("Time count = ");
    lcd.print(timecount);
    lcd.print(" s   ");

    if (digitalRead(B_START) == 0)  {
      delay(db);
      T.SetTimer(0, timecount, 0);
      mode = 1;
    }

    Serial.print(digitalRead(B_START));
    Serial.print("  ");
    Serial.println(digitalRead(B_SET));
    delay(100);
  }
  while (mode == 1) {
    timeCount();
  }
  while (mode == 2) {
    setTimes();
  }
}
