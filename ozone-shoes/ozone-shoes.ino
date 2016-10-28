#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);

#define B_START 8
#define B_SET   9


void setup()
{
  Serial.begin(115200);

  pinMode(B_START, INPUT_PULLUP);
  pinMode(B_SET, INPUT_PULLUP);
  
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("     Welcome    ");
  lcd.setCursor(0, 1);
  lcd.print("   Ozone shoes  ");
  delay(2000);
  lcd.clear();
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Ozone shoes :   ");
  lcd.setCursor(0, 1);
  lcd.print("Time count = ");
  lcd.print(digitalRead(B_START));
  
  Serial.print(digitalRead(B_START));
  Serial.print("  ");
  Serial.println(digitalRead(B_SET));
  delay(100);
}
