#include <Wire.h> 
#include <LiquidCrystal_I2C.h>   
#include <math.h>

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

void drawIndicator(bool red, bool green, bool blue) {
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.setCursor(4,1);
  lcd.print(" ");
  lcd.setCursor(10,1);
  lcd.print(" ");

  if(red) {
    lcd.setCursor(0,1);
    lcd.print(">");
  }

  if(green) {
    lcd.setCursor(4,1);
    lcd.print(">");
  }

  if(blue) {
    lcd.setCursor(10,1);
    lcd.print(">");
  }
}

void output(long red, long green, long blue, int color_index) {
  lcd.setCursor(1,0);
  lcd.print("RED");
  
  lcd.setCursor(5,0);
  lcd.print("GREEN");

  lcd.setCursor(11,0);
  lcd.print("BLUE");

  lcd.setCursor(1,1);
  lcd.print(red);

  lcd.setCursor(5,1);
  lcd.print(green);

  lcd.setCursor(11,1);
  lcd.print(blue);

  switch(color_index){
    case 0: {
      drawIndicator(1, 0, 0);
      break;
    }
    case 1: {
      drawIndicator(0, 1, 0);
      break;
    }
    case 2: {
      drawIndicator(0, 0, 1);
      break;
    }
  }
}

void loop()
{
  output(255, 255, 255, color_index);

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
      color_index = (color_index + 1) % 3;
      // Serial.println("RED");
      
      // lcd.clear();
    }
  }

  lastRedButtonState = redReading;
}