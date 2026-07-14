/*
  LCD1602 I2C Test
  Autonomous Firefighting and Rescue Robot

  Purpose:
  Tests the LCD1602 I2C display separately before integration.

  LCD I2C connections:
  SDA -> GPIO22
  SCL -> GPIO21
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_SDA 22
#define LCD_SCL 21

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin(LCD_SDA, LCD_SCL);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("LCD Test");
  lcd.setCursor(0, 1);
  lcd.print("I2C Working");

  Serial.begin(115200);
  Serial.println("LCD I2C Test Started");
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fire Robot");
  lcd.setCursor(0, 1);
  lcd.print("LCD Working");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SDA: GPIO22");
  lcd.setCursor(0, 1);
  lcd.print("SCL: GPIO21");
  delay(1500);
}
