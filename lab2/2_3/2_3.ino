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
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int counter = 0;
int redFlashesLeft = 0;
int greenFlashesLeft = 0;

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

  printCounter();
}

void printCounter()
{
  Serial.print("Counter: ");
  Serial.print(counter);
  Serial.print("\n");
}

void loop()
{
  if (redFlashesLeft > 0)
  {
    redFlashesLeft--;
    digitalWrite(LED_PIN_RED, LOW);
    delay(150);
    digitalWrite(LED_PIN_RED, HIGH);
    delay(150);
    return;
  }

  int reading = digitalRead(BUTTON_PIN);

  Serial.println(reading);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if(buttonState)

      if (buttonState == HIGH)
      {
        // redFlashesLeft = 1;
        // printCounter();
      } else {

      }
    }
  }

  digitalWrite(LED_PIN_RED, redLedState);

  if(reading != lastButtonState) {
    if(reading == LOW) {
      digitalWrite(LED_PIN_RED, HIGH);
      delay(150);
      digitalWrite(LED_PIN_RED, LOW);
    } else {
      digitalWrite(LED_PIN_GREEN, HIGH);
      delay(150);
      digitalWrite(LED_PIN_GREEN, LOW);
    }
    
  }

  lastButtonState = reading;
}