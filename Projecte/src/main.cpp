/*
* Servidor web con ESP32
* Librerias necesarias para conectarnos a un entorno Wifi y poder configurar  
* un servidor WEB
*/
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

/*
* Sensor BMP280
*/
Adafruit_BMP280 bmp280;
float temperatura;

/*
* Display
*/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



/*
  Pins para el lector RFID
*/
#define RST_PIN	26    //Pin 26 para el reset del RC522
#define SS_PIN	5   //Pin 5 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
String targeta;

 
/* Añade tu SSID & Clave para acceder a tu Wifi */
const char* ssid = "TEST";  // Tu SSID
const char* password = "1122334455";  //Tu Clave
 
/*
    Declaramos un objeto de la libreria WebServer para poder acceder a sus funciones
    Y como parametro 80, que es el puerto estandar de todos los servicios WEB HTTP
*/
WebServer server(80);
 
/*
   Declaramos el estado inicial de los LEDs del ESP32
*/
bool LedSectorAStat = LOW;
uint8_t LEDA1pin = 14;
bool LEDA1Estado = LOW;
uint8_t LEDA2pin = 12;
bool LEDA2Estado = LOW;
uint8_t LEDA3pin = 13;
bool LEDA3Estado = LOW;


bool LedSectorBStat = LOW;
uint8_t LEDB1pin = 2;
bool LEDB1Estado = LOW;
uint8_t LEDB2pin = 4;
bool LEDB2Estado = LOW;
uint8_t LEDB3pin = 16;
bool LEDB3Estado = LOW;
 
/*
   Aqui esta definido todo el HTML y el CSS del servidor WEB con ESP32
*/
String SendHTML(uint8_t LedSectorAStat, uint8_t LedSectorBStat, String targeta, float temperatura) {
  // Cabecera de todas las paginas WEB
  String ptr = "<!DOCTYPE html> <html>\n";
  
  // <meta> viewport. Para que la pagina se vea correctamente en cualquier dispositivo
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Control LED</title>\n";
  
/*
 * El estilo de la pagina WEB, tipo de letra, tamaño, colores, 
 * El estilos de los botones (las clases en CSS) y la definicion de como van a cambiar dependiendo de como
 * cambien los estado de los LEDs, color fondo etc
 */
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "div#left{float: left; margin-left: 200px;}\n";
  ptr += "div#right{float: right; margin-right: 200px;}\n";
  ptr += "div#continuacio{clear: both;}\n";
  ptr += ".button {width: 50px;text-align: center;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 18px;cursor: pointer;margin: 0px auto 20px;border-radius: 4px;transition-duration: 0.4s;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "h1{text-align: center;}\n";
  ptr += "h3{text-align: center;}\n";
  ptr += "h5{font-size: 14px;color: #888;}\n";
  ptr += "h4{font-size: 16px; color #888; margin: 5px;}\n";
  ptr += "fieldset{text-align:center; color: black; border: solid 2px black;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  /*
   * Encabezados de la pagina
   */
  ptr += "<div id=\"header\">\n";
  ptr += "<h1 align=\"center\">ESP32 Servidor WEB</h1>\n";
  ptr += "<h3 align=\"center\">Usando Modo Estacion</h3>\n";
  ptr += "</div>\n";
/*
 * Aqui esta la inteligencia del servidor WEB con ESP32, dependiento de los parametros de la funcion SendHTML
 * modificara la vista de la pagina WEB,  llamaran a las clases "button-on y button-off" que cambian como
 * se muestran los datos en la pagina WEB 
 */
if (targeta == "ab540d41")
{
  ptr += "<fieldset><fieldset><h4>Luces</h4></fieldset>\n";
  
  ptr += "<div id=\"left\">\n";
  ptr += "<h5>Led's Sector A</h5>\n";
  if (LedSectorAStat)
  {
    ptr += "<a class=\"button button-on\" href=\"/LedSectorAoff\">ON</a>";
  }
  else
  {
    ptr += "<a class=\"button button-off\" href=\"/LedSectorAon\">OFF</a>";
  }
  ptr += "</div>\n";

  ptr += "<div id=\"right\">\n";
  ptr += "<h5>Led's Sector B</h5>\n";
  if (LedSectorBStat)
  {
    ptr += "<a class=\"button button-on\" href=\"/LedSectorBoff\">ON</a>";
  }
  else
  {
    ptr += "<a class=\"button button-off\" href=\"/LedSectorBon\">OFF</a>";
  }
  ptr += "</div>\n";
  ptr += "</fieldset>\n";

  ptr += "<div id=\"continuacio\">\n";
  ptr += "<span>Mi Web</span>\n";
  ptr += "</div>\n";
 
}
else 
{
  ptr += "<fieldset><h4>Targeta no reconeguda</fieldset>";
}
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}



/*
   Todos las siguientes funciones ejecutan tres tareas:
   1 Cambian de estado las variables  de los LED
   2 Muestran por el monitor Serial de Arduino IDE, informacion relevante al estado de los LED
   3 Actualizan la vista de la pagina del servidor WEB con ESP32, envia al navegador un codigo 200 indican el exito
   de la conexion y llama a otra funcion SendHTML con dos parametros que modificaran la pagina 
   del servidor WEB con Arduino.
*/

void leer_bmp280(){
  temperatura = bmp280.readTemperature();
}

void handle_OnConnect() {
  LEDA1Estado = LOW; // 1
  LEDA2Estado = LOW; // 1
  LEDA3Estado = LOW;
  LEDB1Estado = LOW;
  LEDB2Estado = LOW;
  LEDB3Estado = LOW;
  LedSectorAStat = LOW;
  LedSectorBStat = LOW;
  leer_bmp280;
  Serial.println("GPIO4 Estado: OFF | GPIO5 Estado: OFF"); // 2
  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura)); // 3
}
 
void handle_LedSectorAon() {
  LEDA1Estado = HIGH; // 1
  LEDA2Estado = HIGH; // 1
  LEDA3Estado = HIGH;
  LedSectorAStat = HIGH;
  leer_bmp280;
  Serial.println("GPIO4 Estado: ON"); // 2
  server.send(200, "text/html", SendHTML(true, LedSectorBStat, targeta, temperatura)); //3
}
 
void handle_LedSectorAoff() {
  LEDA1Estado = LOW; // 1
  LEDA2Estado = LOW; // 1
  LEDA3Estado = LOW;
  LedSectorAStat = LOW;
  leer_bmp280;
  Serial.println("GPIO4 Estado: OFF");
  server.send(200, "text/html", SendHTML(false, LedSectorBStat, targeta, temperatura));
}
 
void handle_LedSectorBon() {
  LEDB1Estado = HIGH;
  LEDB2Estado = HIGH;
  LEDB3Estado = HIGH;
  LedSectorBStat = HIGH;
  leer_bmp280;
  Serial.println("GPIO5 Estado: ON");
  server.send(200, "text/html", SendHTML(LedSectorAStat, true, targeta, temperatura));
}
 
void handle_LedSectorBoff() {
  LEDB1Estado = LOW;
  LEDB2Estado = LOW;
  LEDB3Estado = LOW;
  LedSectorBStat = LOW;
  leer_bmp280;
  Serial.println("GPIO5 Estado: OFF");
  server.send(200, "text/html", SendHTML(LedSectorAStat, false, targeta, temperatura));
}
 
void handle_NotFound() {
  server.send(404, "text/plain", "La pagina no existe");
}
 
void setup() {
  /*
   * Declaracion de la velocidad de comunicacion entre Arduino IDE y ESP32
   * Configura el comportamiento de los pines
   */
  Serial.begin(115200);
  pinMode(LEDA1pin, OUTPUT);
  pinMode(LEDA2pin, OUTPUT);
  pinMode(LEDA3pin, OUTPUT);
  pinMode(LEDB1pin, OUTPUT);
  pinMode(LEDB2pin, OUTPUT);
  pinMode(LEDB3pin, OUTPUT);
/*
* BUS SPI del RFID
*/
  SPI.begin();
  mfrc522.PCD_Init();

/*
* Configuracion del Display
*/
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.println("\nTarget ID: ");

/*
* configuracio BMP280
*/
if (!bmp280.begin(0x76,0x58)) {
Serial.println("Could not find a valid BMP280 sensor, check wiring!");
while (1);
}
bmp280.setSampling(Adafruit_BMP280::MODE_FORCED,     // Operating Mode. 
                   Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling 
                   Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling 
                   Adafruit_BMP280::FILTER_X16,      // Filtering. 
                   Adafruit_BMP280::STANDBY_MS_500); // Standby time. 
leer_bmp280();

/*
 * Configuracion de la conexion a la Wifi de tu casa
 */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
/*
 * Para procesar las solicitudes HTTP necesitamos definir el codigo que debe de ejecutar en
 * cada estado. Para ello utilizamos el metodo "on" de la libreria WebServer que hemos 
 * habilitador en la linea 13 de este codigo
 * 1 El primero se ejecuta cuando te conectas al Servidor WEB con ESP32 http://la_ip_del_esp32/
 * 2 Los 4 siguientes procesan los 2 estados que puede tener cada LED ON/OFF
 * 3 El ultimo gestiona los errores por ejemplo si pones http://la_ip_del_esp32/holaquetal
 * esta pagina no existe, por lo tanto actualizara la pagina WEB con un mensaje de error
 */
  server.on("/", handle_OnConnect); // 1
  server.on("/LedSectorAon", handle_LedSectorAon); // 2
  server.on("/LedSectorAoff", handle_LedSectorAoff); // 2
  server.on("/LedSectorBon", handle_LedSectorBon); // 2
  server.on("/LedSectorBoff", handle_LedSectorBoff); // 2
  server.onNotFound(handle_NotFound); // 3
/*
 * Arrancamos el Servicio WEB
 */
  server.begin();
  Serial.println("Servidor HTTP iniciado");

}
/*
 * Para gestionar las la peticiones HTTP es necesario llamar al metodo "handleClient"
 * de la libreria WebServer que se encarga de recibir las peticiones y lanzar las fuciones
 * de callback asociadas tipo "handle_led1on()" "handle_led2on()" etc
 * Tambien ejecutan el cambio de estado de los pines y por lo tanto hacen que los 
 * LEDs se enciendan o apaguen
 */
void loop() {
  server.handleClient();

	// Revisamos si hay nuevas tarjetas  presentes
	if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
  		//Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
              String tar;
              tar = "";
              targeta = "";
                  // Enviamos serialemente su UID
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          tar += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          tar += String(mfrc522.uid.uidByte[i], HEX);
                          
                  } 
                  Serial.println();
                  for (int i = 0; i < tar.length(); i++)
                  {
                    if(tar[i] == ' '){}
                    else {
                      targeta += tar[i];
                    }
                  }
                  leer_bmp280;
                  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura));
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();  

                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(0, 10);
                  display.clearDisplay();
                  if (targeta =="ab540d41"){
                    display.println("\nUser: Pol");
                  }
                  else {
                    display.println("\nUser: Unknown");
                  }
                  display.print("\n\nID: ");
                  display.print(targeta);
                  display.display();

                  Serial.print(temperatura);
            }      
	}


  if (LedSectorAStat)
  {
    digitalWrite(LEDA1pin, HIGH);
    digitalWrite(LEDA2pin, HIGH);
    digitalWrite(LEDA3pin, HIGH);
  }
  else
  {
    digitalWrite(LEDA1pin, LOW);
    digitalWrite(LEDA2pin, LOW);
    digitalWrite(LEDA3pin, LOW);
  }
 
  if (LedSectorBStat)
  {
    digitalWrite(LEDB1pin, HIGH);
    digitalWrite(LEDB2pin, HIGH);
    digitalWrite(LEDB3pin, HIGH);
  }
  else
  {
    digitalWrite(LEDB1pin, LOW);
    digitalWrite(LEDB2pin, LOW);
    digitalWrite(LEDB3pin, LOW);
  }
}