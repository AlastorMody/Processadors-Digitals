#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>



#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C

#define SCREEN_WIDTH 128 // OLED display ancho, en pixels
#define SCREEN_HEIGHT 64 // OLED display alto, en pixels


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP280 bmp280;
float temperature, pressure, altitude;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  } 
  display.clearDisplay();
  
  if (!bmp280.begin(0x76,0x58)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }
  bmp280.setSampling(Adafruit_BMP280::MODE_FORCED,     // Operating Mode. 
                    Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling 
                    Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling 
                    Adafruit_BMP280::FILTER_X16,      // Filtering. 
                    Adafruit_BMP280::STANDBY_MS_500); // Standby time. 
                    
}

void loop()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) Serial.print("0");
      Serial.print(address,HEX);
      Serial.println(" !");
      nDevices++;    
    }
    else if (error==4)
    {
    Serial.print("Unknown error at address 0x");
    if (address<16)
    {
      Serial.print("0");
      Serial.println(address,HEX);
    }
    }
  }
  if (nDevices == 0) Serial.println("No I2C devices found\n");
  else Serial.println("done\n");
  if (bmp280.takeForcedMeasurement()) {
    // can now print out the new measurements

    temperature = bmp280.readTemperature();
    pressure = bmp280.readPressure();
    altitude = bmp280.readAltitude(1013.25);
    Serial.print(F("Temperature = "));
    Serial.print(temperature);
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(pressure);
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(altitude); // Adjusted to local forecast! 
    Serial.println(" m");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("\nT =  ");
    display.print(temperature);
    display.print((char)247);
    display.println("C\n");
    display.print("P = ");
    display.print(pressure);
    display.println(" Pa\n");
    display.print("Alt = ");
    display.print(altitude);
    display.println(" m");


    display.display();
    display.clearDisplay();

    Serial.println();
    delay(2000);
  } 
  else {
    Serial.println("Forced measurement failed!");
  }
}