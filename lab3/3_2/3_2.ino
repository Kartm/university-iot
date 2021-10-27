#include <Wire.h> 
#include <LiquidCrystal_I2C.h>   
 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_BUTTON_PIN 2

#define RED 6
#define GREEN 5
#define BLUE 3

int redButtonState = HIGH;
int lastRedButtonState = HIGH;

unsigned long lastRedDebounceTime = 0;
unsigned long debounceDelay = 50;

int color_index = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);

  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  initLCD();
}

void initLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void output(long red, long green, long blue, int color_index) {
  lcd.setCursor(0,0);

  lcd.print("  ");
  lcd.print(red);
  
  lcd.print(" ");
  lcd.print(green);

  lcd.print(" ");
  lcd.print(blue);
}

void loop()
{
  output(255, 255, 255);

  int redReading = digitalRead(RED_BUTTON_PIN);

  if (redReading != lastRedButtonState)
  {
    lastRedDebounceTime = millis();
  }

  if ((millis() - lastRedDebounceTime) > debounceDelay)
  {
    if (redReading != redButtonState)
    {
      redButtonState = redReading;
    }
  }

  if(redReading != lastRedButtonState) {
    if(redReading == LOW) {
      Serial.println("RED");
      
      lcd.clear();
    }
  }

  lastRedButtonState = redReading;
}