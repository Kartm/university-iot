#include <Wire.h> 
#include <LiquidCrystal_I2C.h>   
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_BUTTON_PIN 2
#define GREEN_BUTTON_PIN 4

#define RED 6
#define GREEN 5
#define BLUE 3

int greenButtonState = HIGH;
int lastGreenButtonState = HIGH;

int redButtonState = HIGH;
int lastRedButtonState = HIGH;

unsigned long lastGreenDebounceTime = 0;
unsigned long lastRedDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long red_val = 0;
unsigned long green_val = 0;
unsigned long blue_val = 0;

int color_index = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
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

  if(red < 10) {
    lcd.setCursor(2,1);
    lcd.print(" ");
  }

  if(red < 100) {
    lcd.setCursor(3,1);
    lcd.print(" ");
  }

  lcd.setCursor(5,1);
  lcd.print(green);

  if(green < 10) {
    lcd.setCursor(6,1);
    lcd.print(" ");
  }

  if(green < 100) {
    lcd.setCursor(7,1);
    lcd.print(" ");
  }

  lcd.setCursor(11,1);
  lcd.print(blue);

  if(blue < 10) {
    lcd.setCursor(12,1);
    lcd.print(" ");
  }

  if(blue < 100) {
    lcd.setCursor(13,1);
    lcd.print(" ");
  }


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

unsigned long addition = 5;

void loop()
{
  output(red_val, green_val, blue_val, color_index);

  analogWrite(RED, red_val);
  analogWrite(GREEN, green_val);
  analogWrite(BLUE, blue_val);

  int greenReading = digitalRead(GREEN_BUTTON_PIN);
  int redReading = digitalRead(RED_BUTTON_PIN);

if (greenReading != lastGreenButtonState)
  {
    lastGreenDebounceTime = millis();
  }

  if (redReading != lastRedButtonState)
  {
    lastRedDebounceTime = millis();
  }

   if ((millis() - lastGreenDebounceTime) > debounceDelay)
  {
    if (greenReading != greenButtonState)
    {
      greenButtonState = greenReading;
    }
  }

  if ((millis() - lastRedDebounceTime) > debounceDelay)
  {
    if (redReading != redButtonState)
    {
      redButtonState = redReading;
    }
  }

  if(greenReading != lastGreenButtonState) {
    if(greenReading == LOW) {
      switch(color_index){
    case 0: {
      red_val = (red_val + addition) % 256;
      break;
    }
    case 1: {
      green_val = (green_val + addition) % 256;
      break;
    }
    case 2: {
      blue_val = (blue_val + addition) % 256;
      break;
    }
  }
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