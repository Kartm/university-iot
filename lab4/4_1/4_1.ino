#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_BUTTON 2
#define GREEN_BUTTON 4
#define RED_LED 6
#define GREEN_LED 5
#define BLUE_LED 3

#define POTENTIOMETER A0

#define ENCODER1 A2
#define ENCODER2 A3

void setup() {
    Serial.begin(9600);

    lcd.init();
    lcd.clear();
    lcd.backlight();

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
    
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    lcd.clear();
    lcd.print("POTENTIOMETER A0");
    lcd.setCursor(0, 1);
    lcd.print("V=");
    lcd.setCursor(7, 1);
    lcd.print("ADC=");
}

int lastReading = -100;

void loop()
{
    int finalReading = 0;
    int currentReading = analogRead(POTENTIOMETER);

    int delta = abs(currentReading - lastReading);

    // prevents blinking
    if(delta > 2) {
      lastReading = currentReading;

      finalReading = currentReading;
    } else {
      finalReading = lastReading;
    }

    lcd.setCursor(12, 1);

    String voltageStr = "";
    String readingStr = String(finalReading);

    if(readingStr.length() == 1) {
      voltageStr = " 00" + readingStr;
    } if(readingStr.length() == 2) {
      voltageStr = " 0" + readingStr;
    } if(readingStr.length() == 3) {
      voltageStr = " " + readingStr;
    } else  {
      voltageStr = readingStr;
    }


    lcd.print(voltageStr);
    lcd.setCursor(2, 1);

    float scaledPotentiometerValue = (float) finalReading / 1023 * 5;

    lcd.print(String(scaledPotentiometerValue, 2));
}