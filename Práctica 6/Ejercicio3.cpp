#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <Time.h>
#include <TimeLib.h>

#define RST_PIN 0 //Pin 0 para el reset del RC522
#define SS_PIN 5 //Pin 5 para el SS (SDA) del RC522
#define CS 4 //Pin 4 para el SS del lector de SD
time_t fecha;
String rfid;

MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

File myFile;
String alltxt = "";

void WriteFile(const char * path, const char * message){
 
  myFile = SD.open(path, FILE_WRITE);
 
  if (myFile) {
    Serial.printf("Writing to %s ", path);
    Serial.println(message);
    myFile.println(message);
    myFile.close(); 
    Serial.println("completed.");
  } 
  
  else {
    Serial.println("error opening file ");
    Serial.println(path);
  }
}

void writeRFID (String rfid, time_t fecha)
{
    String date;
    const char* message;

        //Serial.println("open file.... registros.log");
        date += "Hora: ";
        date += hour(fecha);
        date += ":";
        date += minute(fecha);
        date += ":";
        date += second(fecha);        
        date += "  ";
        date += day(fecha);
        date += "/";
        date += month(fecha);
        date += "/";
        date += year(fecha);
        date += "      RFID: ";
        date += rfid;
        date += "\n";

        Serial.println(date);

        alltxt += date;

        message = alltxt.c_str();

        Serial.print("msg = ");
        Serial.println(message);

        WriteFile("/registros.log", message);
    
}

void setup()
{
    Serial.begin(115200);
    setTime(15, 32, 00, 07, 06, 2023);
    fecha = now();
    SPI.begin(); //Iniciamos el Bus SPI
    mfrc522.PCD_Init(); // Iniciamos el MFRC522
    Serial.println("Lectura del UID");
    Serial.print("Iniciando SD ...");
  if (!SD.begin(CS)) 
  {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
 
}


void loop()
{
// Revisamos si hay nuevas tarjetas presentes
    fecha = now();
    if ( mfrc522.PICC_IsNewCardPresent())
    {
//Seleccionamos una tarjeta
        if ( mfrc522.PICC_ReadCardSerial())
        {   
// Enviamos serialmente su UID
            Serial.print("Card UID:");
            rfid = "";
            String temp;
            for (byte i = 0; i < mfrc522.uid.size; i++) 
            {
                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522.uid.uidByte[i], HEX);
                temp = String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                temp.toUpperCase();
                temp += String(mfrc522.uid.uidByte[i], HEX);
                temp.toUpperCase();
                rfid += temp;
            }
            Serial.println("\nStringcheck: " + rfid);
// Terminamos la lectura de la tarjeta actual
            mfrc522.PICC_HaltA();
        }
// Escribimos en el archivo log el RFID de la tarjeta y la fecha y hora "actuales"
       
        writeRFID(rfid, fecha);
    }
}