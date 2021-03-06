#define BUTTON_PIN 2
#define LED_PIN_RED 6
#define LED_PIN_GREEN 5
#define LED_PIN_BLUE 3

/*
Write a program that will handle different actions to different states of a button:

when pressed down, it flashes a red LED once
when releasing it flashes a green LED once
when the pushbutton is pressed for a long time - at least 2 seconds - and the green and red LEDs flash alternately
recognises a double-click of the button (two short presses in quick succession) followed by flashing of the blue LED

For all the above situations, the corresponding event recognition messages should be displayed on the terminal.
*/

int redLedState = LOW;
int greenLedState = LOW;
int blueRedState = LOW;

int buttonState;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long pressedInTime = 0;
unsigned long lastPressedTime = 0;

int counter = 0;
int redFlashesLeft = 0;
int greenFlashesLeft = 0;

unsigned long alternateFlashingDelay = 300;

unsigned long clickCounter = 0;


void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);

  digitalWrite(LED_PIN_RED, redLedState);
  digitalWrite(LED_PIN_GREEN, greenLedState);
  digitalWrite(LED_PIN_BLUE, blueRedState);
}

void loop()
{
  if(lastPressedTime > 0 && clickCounter == 2 && millis() - lastPressedTime < 500) {
    Serial.println("Detected double click!");

    digitalWrite(LED_PIN_BLUE, HIGH);
    delay(100);
    digitalWrite(LED_PIN_BLUE, LOW);
    lastPressedTime = 0;
  } else if (millis() - lastPressedTime >= 500) {
    clickCounter = 0;
  }

  if (pressedInTime > 0 && millis() - pressedInTime > 2000)
  {
    Serial.println("Detected long press!");

    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_RED, HIGH);
    delay(alternateFlashingDelay);
    digitalWrite(LED_PIN_GREEN, HIGH);
    digitalWrite(LED_PIN_RED, LOW);
    delay(alternateFlashingDelay);
  }

  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
    }
  }

  digitalWrite(LED_PIN_RED, redLedState);

  if(reading != lastButtonState) {
    if(reading == LOW) {
      Serial.println("Detected button press!");
      pressedInTime = millis();
      lastPressedTime = millis();
      clickCounter++;
      digitalWrite(LED_PIN_RED, HIGH);
      delay(30);
      digitalWrite(LED_PIN_RED, LOW);
    } else {
      Serial.println("Detected button release!");
      pressedInTime = 0;
      digitalWrite(LED_PIN_GREEN, HIGH);
      delay(30);
      digitalWrite(LED_PIN_GREEN, LOW);
    }
    
  }

  lastButtonState = reading;
}