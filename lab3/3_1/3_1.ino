#include <Wire.h> 
#include <LiquidCrystal_I2C.h>   
 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define GREEN_BUTTON_PIN 4
#define RED_BUTTON_PIN 2

/*
Write a program to act as an electronic stopwatch, showing passed time on an LCD display. 

The green key starts and stops the stopwatch. 
The red key stops the stopwatch if it was running, and resets the time value to 0.

Nice to have a precision with miliseconds.
*/

int greenButtonState = HIGH;
int redButtonState = HIGH;

int lastGreenButtonState = HIGH;
int lastRedButtonState = HIGH;

unsigned long lastGreenDebounceTime = 0;
unsigned long lastRedDebounceTime = 0;
unsigned long debounceDelay = 50;

bool running = false;
long msTimeOnWatch = 0;

long millisec = 0;
long tseconds = 0;
long tminutes = 0;
long seconds = 0;


void setup()
{
  Serial.begin(9600);

  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);

  initLCD();

  msTimeOnWatch = 0;

       millisec = 0;
 tseconds = 0;
 tminutes = 0;
 seconds = 0;
}

void initLCD()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void output(long min, long sec, long millisec) {
  
  lcd.setCursor(0,0);
  if(min < 10) {
    lcd.print("0");
  }
  lcd.print(min);
  lcd.print(":");
  if(sec < 10) {
    lcd.print("0");
  }
  lcd.print(sec);
  lcd.print(":");
  if(millisec < 10) {
    lcd.print("0");
  }
  if(millisec < 100) {
    lcd.print("0");
  }
  lcd.print(millisec);
}

long clock_reseted_at = 0;

void loop()
{
  if(running) {
    msTimeOnWatch = millis() - clock_reseted_at;
  } else if(msTimeOnWatch == 0) {
    clock_reseted_at = millis();
  }

  millisec  = msTimeOnWatch % 1000;
  tseconds = msTimeOnWatch / 1000;
  tminutes = tseconds / 60;
  seconds = tseconds % 60;

  output(tminutes, seconds, millisec);

  Serial.println(seconds);

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
      Serial.println("GREEN");

      running = !running;
    }
  }

  if(redReading != lastRedButtonState) {
    if(redReading == LOW) {
      Serial.println("RED");
      running = false;
      msTimeOnWatch = 0;
      clock_reseted_at = millis();

      millisec = 0;
      tseconds = 0;
      tminutes = 0;
      seconds = 0;

      
      lcd.clear();
    }
  }

  lastGreenButtonState = greenReading;
  lastRedButtonState = redReading;
}