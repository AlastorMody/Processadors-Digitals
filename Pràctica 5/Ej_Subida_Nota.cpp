#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <WebServer.h>

#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C

#define SCREEN_WIDTH 128 // OLED display ancho, en pixels
#define SCREEN_HEIGHT 64 // OLED display alto, en pixels


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// float temperature, pressure, altitude;
MAX30105 particleSensor;
#define MAX_BRIGHTNESS 255
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif
int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

const char* ssid = "TEST";  // Enter your SSID here
const char* password = "1122334455";  //Enter your Password here
WebServer server(80); 

String HR = " ";
String SPO2 = " ";

// HTML & CSS contents which display on web server
String HTML = " ";

void html_block() 
{
    HTML += "<!DOCTYPE html>\n";
    HTML += "<html>\n";
    HTML += "<head>\n";
    HTML += "<meta charset='UTF-8'>\n";
    HTML += "<title>PR&Agrave;CTICA 4</title>\n";
    HTML += "<style>\n";
    HTML += "display: none;\n";
    HTML += "}\n";
    HTML += "</style>\n";
    HTML += "<link rel='stylesheet' href='https://cdn.jsdelivr.net/gh/Microsoft/vscode/extensions/markdown-language-features/media/markdown.css'>\n";
    HTML += "<link rel='stylesheet' href='https://cdn.jsdelivr.net/gh/Microsoft/vscode/extensions/markdown-language-features/media/highlight.css'>\n";
    HTML += "<style>\n";
    HTML += " body {\n";
    HTML += "font-family: -apple-system, BlinkMacSystemFont, 'Segoe WPC', 'Segoe UI', system-ui, 'Ubuntu', 'Droid Sans', sans-serif;\n";
    HTML += "font-size: 14px;\n";
    HTML += "line-height: 1.6;\n";
    HTML += "}\n";
    HTML += "</style>\n";
    HTML += "<style>\n";
    HTML += ".task-list-item {\n";
    HTML += "list-style-type: none;\n";
    HTML += "}\n";
    HTML += ".task-list-item-checkbox {\n";
    HTML += "margin-left: -20px;\n";
    HTML += "vertical-align: middle;\n";
    HTML += "pointer-events: none;\n";
    HTML += "}\n";
    HTML += "</style>\n";
    HTML += "</head>\n";
    HTML += "<body class='vscode-body vscode-light'>\n";
    HTML += "<h1 id='pràctica-5'>PRÀCTICA 5</h1>\n";
    HTML += "<h2 id='Heartrate'>Puslacions i oxigen en sang</h2>\n";
    HTML += "<p>HR: ";
    HTML += HR;
    HTML += "  SPO2: ";
    HTML += SPO2;
    HTML +=  "</p>";
    HTML += "</body>\n";
    HTML += "</html>";
}

  // Handle root url (/)
  void handle_root() {
  server.send(200, "text/html", HTML);
}

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
  
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);


  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.begin();
  Serial.println("HTTP server started");
  delay(100);

}

void loop()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps
 //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
  }
  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }
    //take 25 sets of samples before calculating the heart rate.
    HR = " "; SPO2 = " ";
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data
      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
      //send samples and calculation result to terminal program through UART

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);

      if(validHeartRate == 1)
      {
        Serial.print(F("HR: "));
        Serial.print(heartRate, DEC);
        Serial.print(F(" pps, "));
        display.print("\nHR: ");
        HR = heartRate;
        display.print(heartRate, DEC);
        display.print(" pps.");
        display.println("\n");
      }
      else 
      {
        Serial.print(F("HR: ---"));
        Serial.print(F(" pps, "));
        display.print("\nHR: ---");
        HR = "---";
        display.print(" pps.");
        display.println("\n"); 
      }
      if(validSPO2 == 1)
      {
        Serial.print(F("SPO2: "));
        Serial.print(spo2, DEC);
        Serial.print(F(" %, "));
        display.print("\nSPO2: ");
        display.print(spo2, DEC);
        SPO2 = spo2;
        display.print(" %.");
        display.println("\n");        
      }
      else
      {
        Serial.print(F("SPO2: --"));
        Serial.print(F(" %, "));
        display.print("\nSPO2: --");
        SPO2 = "---";
        display.print(" %.");
        display.println("\n");            
      }

      display.display();
      display.clearDisplay();
      Serial.println();
      server.handleClient();
    }

    HTML = "";
    html_block();
    server.send(200, "text/html", HTML);
    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}