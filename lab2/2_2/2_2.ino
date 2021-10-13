#define BUTTON_RED_PIN 2
#define BUTTON_GREEN_PIN 4
#define LED_PIN 6

int ledState = LOW;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastPushTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_GREEN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, ledState);
}

void loop() {
  int reading = digitalRead(BUTTON_RED_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        lastPushTime = millis();
        ledState = !ledState;
      } else {
        unsigned long ellapsed = millis() - lastPushTime;
        // H:MM:SS.z

        unsigned long milliseconds = (ellapsed % 1000);
        unsigned long seconds = (ellapsed / 1000) % 60;
        unsigned long minutes = (ellapsed / (1000 * 60)) % 60;
        unsigned long hours = (ellapsed / (1000 * 60 * 60)) % 24;

        Serial.print("Button pressed: ");
        Serial.print(ellapsed);
        Serial.print("ms -> ");

        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(":");
        Serial.print(seconds);
        Serial.print(":");
        Serial.println(milliseconds);
      }
    }
  }

  digitalWrite(LED_PIN, ledState);

  lastButtonState = reading;
}