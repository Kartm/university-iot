#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
 
#define ONE_WIRE_BUS A1
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// uint8_t outside_sensor_address[8] = { 0x28, 0xEE, 0xD5, 0x64, 0x1A, 0x16, 0x02, 0xEC };
// uint8_t inside_sensor_address[8] = { 0x28, 0x61, 0x64, 0x12, 0x3C, 0x7C, 0x2F, 0x27 };

DeviceAddress outside_sensor_address;
DeviceAddress inside_sensor_address;

float outside_temperature = 0;
float inside_temperature = 0;

void printAddress(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("");
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print((char)176);
  Serial.print("C  |  ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print((char)176);
  Serial.println("F");
}
 
void printSensorAddresses() {
  delay(100);
  
  oneWire.reset_search(); //reset search state

  Serial.println("Locating devices...");
  Serial.print("Found ");
  int deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");

  Serial.println("Printing addresses...");
  for (int i = 0;  i < deviceCount;  i++)
  {
    if(i == 0) {
      Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    sensors.getAddress(outside_sensor_address, i);
    printAddress(outside_sensor_address);
    } else if (i == 1) {
      Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    sensors.getAddress(inside_sensor_address, i);
    printAddress(inside_sensor_address);
    }
    
  }
}

void initLcd() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.clear();
  lcd.home();
}

void drawLcd() {
  lcd.setCursor(0, 0);

  lcd.print(String(outside_temperature, 1));

  lcd.setCursor(0, 1);

  lcd.print(String(inside_temperature, 1));
}

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();

  printSensorAddresses();
  initLcd();

  drawLcd();
}
 
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  // Serial.print("Requesting temperatures...");
  // sensors.requestTemperatures(); // Send the command to get temperatures
  // Serial.println("DONE");
  // // After we got the temperatures, we can print them here.
  // // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  // Serial.print("Temperature for the device 1 (index 0) is: ");
  // Serial.println(sensors.getTempCByIndex(0));  

  sensors.requestTemperatures();
  
  Serial.print("Outside sensor: ");
  printTemperature(outside_sensor_address);
  
  Serial.print("Inside sensor: ");
  printTemperature(inside_sensor_address);
  
  Serial.println();
  delay(1000);
}