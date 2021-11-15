#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_BUTTON 2
#define GREEN_BUTTON 4
#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3
#define ENCODER_A A2
#define ENCODER_B A3
#define DEBOUNCE_ENCODER 100

unsigned long debounceDelay = 50;
bool redPrevious;
unsigned long redTime;
bool lockRED;
bool redState()
{
    bool redState = digitalRead(RED_BUTTON) == LOW;
    if(redState != redPrevious)
    {
        redTime = millis();
    }
    redPrevious = redState;
    if(millis() - redTime > debounceDelay)
    {
        if(redPrevious) return true;
    }
    return false;
}

bool greenPrevious;
unsigned long greenTime;
bool lockGREEN;
bool greenState()
{
    bool greenState = digitalRead(GREEN_BUTTON) == LOW;
    if(greenState != greenPrevious)
    {
        greenTime = millis();
    }
    greenPrevious = greenState;
    if(millis() - greenTime > debounceDelay)
    {
        if(greenPrevious) return true;
    }

    return false;
}

bool red()
{
  bool pressed = redState();
  if(!lockRED && pressed)
  {
    lockRED = true;
    return true;
  }
  lockRED = pressed;
  return false;
}

bool green()
{
  bool pressed = greenState();
  if(!lockGREEN && pressed)
  {
    lockGREEN = true;
    return true;
  }
  lockGREEN = pressed;
  return false;
} 


String pointer = "-> ";

class lcdPrinter
{
public:
    static void printOnLcd(String menuName, String subMenu)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(menuName);
        lcd.setCursor(0, 1);
        lcd.println(pointer);
        lcd.setCursor(3, 1);
        lcd.print(subMenu);
    }
};

struct CustomMenu
{
    CustomMenu(const String &option, int subMenusNumber, CustomMenu *subMenus) : name(option), children(subMenus), size(subMenusNumber)                                                                                         
    {}

    CustomMenu(const String &name, void (*func)()) : name(name), func(func)
    {}

    CustomMenu()
    {}

private:
    String name;
    CustomMenu *children;
    int size = 0;
    int currIndex{};

    void (*func)(){};


private:

    bool isOpenSubMenu{};
private:
    void setIndex(int index)
    {
        if (index < 0) index = size - 1;
        else if (index >= size) index = 0;
        currIndex = index;
    }

    bool hasSubMenus() const
    {
        return size > 0;
    }

    String *getNameArr() const
    {
        String name_array[size+1];
        name_array[1] = name;
        for (int i = 0; i < size; i++)
        {
            name_array[i+1] = children[i].name;
        }

        return name_array;
    }

public:
    int nextOption()
    {
        if (!isOpenSubMenu)
        {
            return 1;
        }
        int nextChild = children[currIndex].nextOption();
        setIndex(currIndex + nextChild);
        if (nextChild == 1)
        {
            lcdPrinter::printOnLcd(name, children[currIndex].name);
        }
        return 0;
    }

    int previousOption()
    {
        if (!isOpenSubMenu)
        {
            return -1;
        }
        int prevChild = children[currIndex].previousOption();
        setIndex(currIndex + prevChild);
        if (prevChild == -1)
        {
            lcdPrinter::printOnLcd(name, children[currIndex].name);
        }
        return 0;
    }

    String *goDeeper()
    {
        if (!hasSubMenus())
        {
            func();
            return nullptr;
        }

        if (isOpenSubMenu)
        {
            String *name_array = children[currIndex].goDeeper();
            if (name_array == nullptr)
            {
                return getNameArr();
            }
            return name_array;
        }
        isOpenSubMenu = true;
        lcdPrinter::printOnLcd(name, children[currIndex].name);
        return getNameArr();
    }

    String *goUp()
    {
        if (!hasSubMenus())
        {
            return nullptr;
        }

        if (isOpenSubMenu)
        {
            String *subMenuName = children[currIndex].goUp();
            if (subMenuName == nullptr)
            {
                isOpenSubMenu = false;
                return &name;
            }
            lcdPrinter::printOnLcd(name, children[currIndex].name);
            return &name;
        }

        return nullptr;
    }
};

int ledLight[3] = {255, 255, 255};

void ledOn()
{
    analogWrite(LED_RED, ledLight[0]);
    analogWrite(LED_GREEN, ledLight[1]);
    analogWrite(LED_BLUE, ledLight[2]);
}

void ledOff()
{
    analogWrite(LED_RED, 0);
    analogWrite(LED_GREEN, 0);
    analogWrite(LED_BLUE, 0);
}

void ChangeRed()
{
    int val = ledLight[0];
    while (greenState())
    {
        val++;
        if (val > 255) val -= 255;
        ledLight[0] = val;
        analogWrite(LED_RED, val);
    }
}

void ChangeGreen()
{
    int val = ledLight[1];
    while (greenState())
    {
        val++;
        if (val > 255) val -= 255;
        ledLight[1] = val;
        analogWrite(LED_GREEN, val);
    }
}

void ChangeBlue()
{
    int val = ledLight[2];
    while (greenState())
    {
        val++;
        if (val > 255) val -= 255;
        ledLight[2] = val;
        analogWrite(LED_BLUE, val);
    }
}

void turnOnBacklight()
{
    lcd.backlight();
}

void turnOffBacklight()
{
    lcd.noBacklight();
}

const String celcius = "C ";
const String farenheit = "F ";
String temperatureUnit = celcius;

float getTemperature(float temperature)
{
    if (temperatureUnit == celcius) return temperature;
    return temperature * 9 / 5 + 32;  //conversion to F
}

void temperatureSensorIn()
{
    float temperature = getTemperature(69);
    lcdPrinter::printOnLcd("Temperature", String(temperature) + " " + temperatureUnit);
}

void temperatureSensorOut()
{
    float temperature = getTemperature(23);
    lcdPrinter::printOnLcd("Temperature", String(temperature) + " " + temperatureUnit);
}

void changeToC()
{
    temperatureUnit = celcius;
}

void changeToF()
{
    temperatureUnit = farenheit;
}
void caretPointer()
{
  pointer = ">  ";
}

void arrowPointer()
{
    pointer = "-> ";
}

void dashPointer()
{
    pointer = "-  ";
}

void aboutPage()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("About");
    lcd.setCursor(0, 1);
    lcd.println("Marcin Kasperski");
}

volatile int encoderLastFrame = 0;
volatile long lastInterrupt = 0;

ISR(PCINT1_vect)
{
    int currentStateA = digitalRead(ENCODER_A);
    if (currentStateA != LOW)
        return;

    noInterrupts();
    unsigned long interruptTime = millis();

    if (interruptTime - lastInterrupt > DEBOUNCE_ENCODER)
    {
        if (digitalRead(ENCODER_B) == LOW)
            encoderLastFrame = 1;
        else
            encoderLastFrame = -1;

        lastInterrupt = interruptTime;
    }
    interrupts();
}

CustomMenu Power ("Power", 2, new (CustomMenu[2]) {{"On",  ledOn}, {"Off", ledOff}});
CustomMenu Red ("Red", ChangeRed);
CustomMenu Green ("Green", ChangeGreen);
CustomMenu Blue ("Blue", ChangeBlue);
CustomMenu LEDOptions ("LED Options", 4, new (CustomMenu[4]) {Power, Red, Green, Blue});

CustomMenu Backlight ("Backlight", 2, new (CustomMenu[2]) {{"On",  turnOnBacklight}, {"Off", turnOffBacklight}});
CustomMenu Pointer ("Pointer",  3, new (CustomMenu[3]) {{"> ", caretPointer}, {"- ", dashPointer}, {"->", arrowPointer}});
CustomMenu LCDDisplay ("LCD Display", 2, new (CustomMenu[2]) {Backlight, Pointer});

CustomMenu SensorIn ("Sensor IN",  temperatureSensorIn);
CustomMenu SensorOut ("Sensor OUT",  temperatureSensorOut);
CustomMenu Units ("Units", 2, new (CustomMenu[2]) {{"C ", changeToC}, {"F ", changeToF}});
CustomMenu Temperature ("Temperature", 3, new (CustomMenu[3]) {SensorIn, SensorOut, Units});

CustomMenu About ("About", 1, new (CustomMenu[1]) {{"Author", aboutPage}});

CustomMenu mainMenu ("Main Menu", 4, new (CustomMenu[4]) {LEDOptions, LCDDisplay, Temperature, About});

void setup()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    initInOut();
    Serial.begin(9600);
    
    PCICR |= (1 << PCIE1);      //enable PCMSK1 scan
    PCMSK1 |= (1 << PCINT10);    //Set pin A2 on state change to trigger an interrupt.

    mainMenu.goDeeper();
}

void loop()
{
    static int encoderRead;
    noInterrupts();
    encoderRead = encoderLastFrame;
    encoderLastFrame = 0;
    interrupts();
    if (encoderRead == 1)
    {
        mainMenu.nextOption();
    } else if (encoderRead == -1)
    {
        mainMenu.previousOption();
    }
    if (green())
    {
        mainMenu.goDeeper();
    } else if (red())
    {
        mainMenu.goUp();
    }
}