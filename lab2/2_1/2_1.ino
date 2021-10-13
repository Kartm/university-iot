#define BUTTON_RED_PIN 2
#define BUTTON_GREEN_PIN 4
#define LED_PIN 6

int ledState = LOW;
int redButtonState;
int greenButtonState;
int lastRedButtonState = LOW;
int lastGreenButtonState = LOW;

unsigned long lastRedDebounceTime = 0;
unsigned long lastGreenDebounceTime = 0;
unsigned long debounceDelay = 50;

int counter = 0;
int ledFlashesLeft = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_GREEN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, ledState);

  printCounter();
}

void printCounter() {
  Serial.print("Counter: ");
  Serial.print(counter);
  Serial.print("\n");
}

void loop() {
  if(ledFlashesLeft > 0) {
    ledFlashesLeft--;
    digitalWrite(LED_PIN, LOW);
    delay(150);
    digitalWrite(LED_PIN, HIGH);
    delay(150);
    return;
  }

  int readingRed = digitalRead(BUTTON_RED_PIN);
  int readingGreen = digitalRead(BUTTON_GREEN_PIN);

  if (readingRed != lastRedButtonState) {
    lastRedDebounceTime = millis();
  }
  if (readingGreen != lastGreenButtonState) {
    lastGreenDebounceTime = millis();
  }

  if ((millis() - lastRedDebounceTime) > debounceDelay) {
    if (readingRed != redButtonState) {
      redButtonState = readingRed;

      if (redButtonState == HIGH) {
        counter--;
        printCounter();
      }
    }
  }

  if ((millis() - lastGreenDebounceTime) > debounceDelay) {
    if (readingGreen != greenButtonState) {
      greenButtonState = readingGreen;

      if (greenButtonState == HIGH) {
        counter++;
        printCounter();
      }
    }
  }

  digitalWrite(LED_PIN, ledState);

  lastRedButtonState = readingRed;
  lastGreenButtonState = readingGreen;

  if(lastRedButtonState == 0 && lastGreenButtonState == 0) {
    if(counter >= 0) {
      ledFlashesLeft = counter;
    } else if(counter < 0) {
      ledFlashesLeft = (-1) * counter;
    }

    Serial.print("Flashing the LED ");
    Serial.print(ledFlashesLeft);
    Serial.print(" times!\n");
  }
}