#include "AudioFileSourceICYStream.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
#include "WiFi.h"

// Enter your WiFi setup here:
const char* ssid = "MiFibra-BC0E"; // Enter your SSID here
const char* password = "hArN674S"; //Enter your Password here

const char *URL = "http://strm112.1.fm/blues_mobile_mp3";

AudioGenerator *audio;
AudioFileSourceHTTPStream *file;
AudioOutputI2S *out;

void setup() 
{

    Serial.begin(9600);

    delay(1000);

    WiFi.begin(ssid, password);

    // Try to connect to Wifi forever
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("...Connecting to WiFi");
        delay(1000);
    }
    Serial.println("Connected");

    audioLogger = &Serial;
    file = new AudioFileSourceICYStream(URL);
    file->SetReconnect(5, 0);
    out = new AudioOutputI2S();
    out -> SetGain(0.125);
    out -> SetPinout(26,25,22);
    audio = new AudioGeneratorMP3();
    audio -> begin(file, out);
}

void loop() 
{
    
    if (audio->isRunning())
    {
        audio->loop();
    }
}