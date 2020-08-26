
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1052.75)
const int chipSelect = 4;
static const int RXPin = 5, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
File dataFile;
Adafruit_BME680 bme; // I2C
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  
  Serial.print("GPS and enviroment data logger v1.0 ");
  Serial.println();
  ss.begin(GPSBaud);
  !bme.begin();
  !SD.begin(chipSelect); 
  {
    }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}
void loop() {
  
  Serial.print(bme.temperature);
  Serial.print(" ");
  Serial.print(bme.pressure / 100.0);
  Serial.print(" ");
  Serial.print(bme.humidity);
  Serial.print(" ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.print(" ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.print(" ");
  
  
  while (ss.available() > 0)
    gps.encode(ss.read());
    Serial.print(gps.location.lat(), 6);
 Serial.print(" , ");
 Serial.print(gps.location.lng(), 6);
 Serial.println();
delay(1000);
 
File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
   dataFile.print(bme.temperature);
   dataFile.print(" ");
   dataFile.print(bme.pressure / 100.0);
   dataFile.print(" ");
   dataFile.print(bme.humidity);
   dataFile.print(" ");
   dataFile.print(bme.gas_resistance / 1000.0);
   dataFile.print(" ");
   dataFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
   dataFile.print(" ");
   dataFile.print(gps.location.lat(), 6);
   dataFile.print(" , ");
   dataFile.print(gps.location.lng(), 6);  
   dataFile.println("");
    dataFile.close();
    }
    else {
    Serial.println("error opening datalog.txt");
  }
  delay(1000);
}
