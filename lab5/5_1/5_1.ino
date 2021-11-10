#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_BUTTON 2
#define GREEN_BUTTON 4
#define RED_LED 6
#define GREEN_LED 5
#define BLUE_LED 3

#define POTENTIOMETER A0

#define ENCODER1 A2
#define ENCODER2 A3
#define ENCODER_DEBOUNCE 100

int selectedMenuItemIndex = 0;
int selectedSubmenuItemIndex = -1;

char* ledSubmenu[] = {"Power", "Red", "Green", "Blue"};
char* displaySubmenu[] = {"Backlight", "Selector"};
char* temperature[] = {"Sensor IN", "Sensor OUT", "Units"};
char* about[] = {"Lukasz Blachnicki"};

char** menus[] ={ ledSubmenu, displaySubmenu, temperature, about};
char * menuTitles[] = {"LED options", "Display", "Temperature", "About"};

void setup() {
    Serial.begin(9600);

    PCICR |= (1 << PCIE1);      //enable PCMSK1 scan
    PCMSK1 |= (1 << PCINT10);    //Pin A interrupt. Set pin A2 to trigger an interrupt on state change.

    lcd.init();
    lcd.clear();
    lcd.backlight();

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
    
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    lcd.clear();
    lcd.home();

    selectedMenuItemIndex = -1;
    selectedSubmenuItemIndex = -1;
    drawMenu();
}

int lastFrameEncoderRead = 0;
unsigned long lastInterruptTime = 0;

ISR(PCINT1_vect)
{
    int currentStateA = digitalRead(ENCODER1);
    if (currentStateA != LOW)
        return;

    noInterrupts();
    unsigned long interruptTime = millis();

    if (interruptTime - lastInterruptTime > ENCODER_DEBOUNCE)
    {
        if (digitalRead(ENCODER2) == LOW)
            lastFrameEncoderRead = 1;
        else
            lastFrameEncoderRead = -1;

        lastInterruptTime = interruptTime;
    }
    interrupts();
}

void drawMenu() {
  lcd.setCursor(0, 0);

  // test vals
  // selectedMenuItemIndex = 0;
  // selectedSubmenuItemIndex = 0;

  if(selectedMenuItemIndex != -1) {
    lcd.print(menuTitles[selectedMenuItemIndex]);
  } else if(selectedSubmenuItemIndex != -1) {
    char *submenu = menus[selectedMenuItemIndex][selectedSubmenuItemIndex];

    lcd.print(submenu);
  }
}


int lastReading = 0;

void loop()
{
  if(lastFrameEncoderRead != 0) {
    Serial.println(lastFrameEncoderRead);
      

      if(lastFrameEncoderRead == 1) {
        selectedMenuItemIndex = (selectedMenuItemIndex + 1) % 4;
      } else {
        selectedMenuItemIndex = (selectedMenuItemIndex - 1) % 4;
      }

      lastFrameEncoderRead = 0;
  }
  
    drawMenu();
    int finalReading = 0;
    int currentReading = analogRead(POTENTIOMETER);

    int delta = abs(currentReading - lastReading);

    // prevents blinking
    if(delta > 2) {
      lastReading = currentReading;

      finalReading = currentReading;
    } else {
      finalReading = lastReading;
    }

    // lcd.setCursor(2, 1);

    // lcd.print(String(scaledPotentiometerValue, 2));
}