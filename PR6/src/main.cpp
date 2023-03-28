#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WebServer.h>
#include <String>

File myFile;



#define RST_PIN 9 //Pin 9 para el reset del RC522
#define SS_PIN 21 //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

// SSID & Password
const char* ssid = "Ma7rix"; // Enter your SSID here
const char* password = "castellnou"; //Enter your Password here
WebServer server(80); // Object of WebServer(HTTP port, 80 is defult)

String UUID = "";
String HTML = "";

//HTML Block
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
    HTML += "<h1 id='pràctica-4'>PRÀCTICA 4A</h1>\n";
    HTML += "<h2 id='modificació-duna-web'>MODIFICACIÓ D'UNA WEB</h2>\n";
    HTML += "<p>Apartat d'exemple que representa una explicació sense cap significat, la seva premisa es única i exclusivament escriure una cosa diferent del primer exemple i confirmar que el canvi es produeix en el nostre servidor web. Posarem una imatge per fer més divertit aquest sub-apartat de la pràctica.</p>\n";
    HTML += "<p><img src='https://i.pinimg.com/564x/93/2a/c0/932ac0ca80eb1da0aff1c063f620ef11.jpg' alt=''></p>\n";
    HTML += "<h2 id='conexions-uuid'>CONEXIONS UUID</h2>";
    HTML += "<p>UUID:";
    HTML += UUID;
    HTML +=  "</p>";
    HTML += "</body>\n";
    HTML += "</html>";
}

// Handle root url 
void handle_root() 
{
    server.send(200, "text/html", HTML);
}

void setup()
{
  Serial.begin(115200);
  SPI.begin(); //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Lectura del UID");
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  //Serial.print("Got IP: ");
  //Serial.println(WiFi.IP());
  Serial.print("Got LOCAL IP: ");
  Serial.println(WiFi.localIP()); //Show ESP32 IP on serial
  html_block();
  server.on("/", handle_root);
  server.begin();
  Serial.println("HTTP server started");  
  Serial.print("Iniciando SD ...");

  if (!SD.begin(15)) 
  {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
  myFile = SD.open("archivo.txt");//abrimos el archivo
  if (myFile) 
  {
    Serial.println("archivo.txt:");
    while (myFile.available()) 
    {
      Serial.write(myFile.read());
    }
    myFile.close(); //cerramos el archivo
  } 
  else 
  {
    Serial.println("Error al abrir el archivo");
  }
}
void loop()
{
    server.handleClient();
  // Revisamos si hay nuevas tarjetas presentes
  
    if (mfrc522.PICC_IsNewCardPresent())
    {
        //Seleccionamos una tarjeta
        if (mfrc522.PICC_ReadCardSerial())
        {
            // Enviamos serialemente su UID
            Serial.print("Card UID:");
            //Serial.println(mfrc522.uid.size);
            UUID += "<p>";
            for (byte i = 0; i < mfrc522.uid.size; i++) 
            {    

                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522.uid.uidByte[i], HEX);
                
                UUID += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                UUID += String(mfrc522.uid.uidByte[i], HEX);
                
            }
            UUID += "</p>";
            Serial.println();
            // Terminamos la lectura de la tarjeta actual
            mfrc522.PICC_HaltA();
        }
    }
  HTML = "";
  html_block();
  server.send(200, "text/html", HTML);
    
}