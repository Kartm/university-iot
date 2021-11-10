#include <OneWire.h>
#include <DallasTemperature.h>
 
#define ONE_WIRE_BUS A1
 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
void printSensorAddresses() {
  delay(100);
  
  oneWire.reset_search(); //reset search state
  byte address[8];        //variable used to store address
  int sensors = 0;        //counter for sensors
 
  //search loop
  while(oneWire.search(address)){
 
    //check address checksum
    if (OneWire::crc8(address, 7) != address[7]){
      Serial.println(F("Incorrect OneWire address, check connections"));
      break;
    }
    sensors++;
    Serial.print("Sensor ");
    Serial.print(sensors);
    Serial.print(": ");
 
    //printing sensor type depending on first byte of an address
    switch(address[0]){
      case 0x10: Serial.print("[DS18S20] "); break;
      case 0x28: Serial.print("[DS18B20] "); break;
      case 0x22: Serial.print("[DS1822]  "); break;
      case 0x3B: Serial.print("[DS1825]  "); break;
      default:   Serial.print("[Other]   "); break;
    }
 
    //print sensor address
    for (byte i=0; i<8; i++)
    {
      Serial.print(F("0x"));
      if (address[i] < 0x10) 
        Serial.print("0");
      Serial.print(address[i], HEX);
      Serial.print(" ");
    }
    
    Serial.println();
  }
}

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  printSensorAddresses();
}
 
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  
}