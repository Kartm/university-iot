#define LED 6

unsigned long debounceDelay = 50;

bool led_state = LOW;
bool blinking = false;

int blinkDelay = 50;

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, led_state);

  if (blinking) {
    digitalWrite(LED, HIGH);
    delay(blinkDelay);
    digitalWrite(LED, LOW);
    delay(blinkDelay);
  }

  while (Serial.available() > 0) {
    String ledPart = Serial.readStringUntil(' ');
    String actionPart = Serial.readStringUntil(' ');
    String argumentPart = Serial.readStringUntil('\0');

    int delay_in_ms = argumentPart.toInt();

    if (ledPart != "LED") {
      Serial.println("Command not beginning with \"LED\".");
      return;
    }

    if (actionPart.startsWith("ON")) {
      led_state = HIGH;
      blinking = false;
    } else if (actionPart.startsWith("OFF")) {
      led_state = LOW;
      blinking = false;
    } else if (actionPart.startsWith("BLINK")) {
      if (argumentPart.length() > 0) {
        blinkDelay = delay_in_ms;
      }

      blinking = true;
    } else {
      Serial.println("Argument not recognized. Possible values: \"ON\", \"OFF\", \"BLINK\", \"BLINK [ms]\".");
    }
  }
}