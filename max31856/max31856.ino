

#include <Adafruit_MAX31856.h>
#include <SPI.h>
#include <SD.h>

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo1 = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo1 = Adafruit_MAX31856(10);

Adafruit_MAX31856 maxthermo2 = Adafruit_MAX31856(9, 11, 12, 13);

Adafruit_MAX31856 maxthermo3 = Adafruit_MAX31856(8, 11, 12, 13);


void setup() {

  
  Serial.begin(115200);

  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  Serial.println("MAX31856 thermocouple test");

  maxthermo1.begin();

  maxthermo1.setThermocoupleType(MAX31856_TCTYPE_K);

//  Serial.print("Thermocouple type: ");
//  switch (maxthermo1.getThermocoupleType() ) {
//    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
//    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
//    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
//    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
//    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
//    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
//    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
//    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
//    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
//    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
//    default: Serial.println("Unknown"); break;
// }


}

void loop() {
  Serial.print("Requesting temperatures...");

  
  Serial.print("Cold Junction Temp: "); 
  Serial.println(maxthermo1.readCJTemperature());

  Serial.print("Thermocouple Temp: "); 
  Serial.println(maxthermo1.readThermocoupleTemperature());

  float cvtTemp1 = maxthermo1.readThermocoupleTemperature();
  float cvtTemp2 = maxthermo2.readThermocoupleTemperature();
  float cvtTemp3 = maxthermo3.readThermocoupleTemperature();
  

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File tempFile = SD.open("cvtTemp.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (tempFile) {
    tempFile.println(cvtTemp1);
    tempFile.println(cvtTemp2);
    tempFile.println(cvtTemp3);
    
    tempFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening cvtTemp.txt");
  }
  
  // Check and print any faults
  uint8_t fault = maxthermo1.readFault();
  if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
  }
  delay(1000);
}
