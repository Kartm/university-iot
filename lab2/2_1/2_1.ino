#define BUTTON_RED_PIN 2
#define BUTTON_GREEN_PIN 4
#define LED_PIN 13

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int redButtonState;             // the current reading from the input pin
int greenButtonState;             // the current reading from the input pin
int lastRedButtonState = LOW;   // the previous reading from the input pin
int lastGreenButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastRedDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastGreenDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int counter = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_GREEN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // set initial LED state
  digitalWrite(LED_PIN, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int readingRed = digitalRead(BUTTON_RED_PIN);
  int readingGreen = digitalRead(BUTTON_GREEN_PIN);

  Serial.println(counter);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingRed != lastRedButtonState) {
    // reset the debouncing timer
    lastRedDebounceTime = millis();
  }
  if (readingGreen != lastGreenButtonState) {
    // reset the debouncing timer
    lastGreenDebounceTime = millis();
  }

  if ((millis() - lastRedDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingRed != redButtonState) {
      redButtonState = readingRed;

      // only toggle the LED if the new button state is HIGH
      if (redButtonState == HIGH) {
        counter--;
        ledState = !ledState;
      }
    }
  }

  if ((millis() - lastGreenDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingGreen != greenButtonState) {
      greenButtonState = readingGreen;

      // only toggle the LED if the new button state is HIGH
      if (greenButtonState == HIGH) {
        counter++;
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(LED_PIN, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastRedButtonState = readingRed;
  lastGreenButtonState = readingGreen;
}