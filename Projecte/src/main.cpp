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
#include <Servo.h> 

/*
* Motor Servo SG90
*/
Servo servo;
int angle = 60;
bool EstadoPuerta = LOW;


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
#define RST_PIN	27    //Pin 26 para el reset del RC522
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
uint8_t LEDA2pin = 14;
bool LEDA2Estado = LOW;
uint8_t LEDA3pin = 13;
bool LEDA3Estado = LOW;


bool LedSectorBStat = LOW;
uint8_t LEDB1pin = 2;
bool LEDB1Estado = LOW;
uint8_t LEDB2pin = 4;
bool LEDB2Estado = LOW;
uint8_t LEDB3pin = 17;
bool LEDB3Estado = LOW;
 
/*
   Aqui esta definido todo el HTML y el CSS del servidor WEB con ESP32
*/
String SendHTML(uint8_t LedSectorAStat, uint8_t LedSectorBStat, String targeta, float temperatura, uint8_t EstadoPuerta) {
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
  ptr += "div#leftl{width: 300px; float: left; margin-left: 75px; margin-top: 20px;}\n";
  ptr += "div#right{float: right; margin-right: 200px;}\n";
  ptr += "div#rightr{width: 300px; float: right; margin-right: 75px; margin-top: 20px;}\n";
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
  ptr += "p{float: left; margin-left: 50px; margin-right: 85px}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  /*
   * Encabezados de la pagina
   */
  ptr += "<div id=\"header\">\n";
  ptr += "<h1 align=\"center\">CASA</h1>\n";
  ptr += "<p><img src='https://lh3.googleusercontent.com/pw/AJFCJaVRWDJ4j-uNPBZANBcSTnUMzGjy8XL_0FHzj2-MYRR84BwW8pwHITB4-7dNBoj447y7HJxkfEmZ4VCqEKb2bVHnsCc-FmXq3ihvwbLNWfplw9CtzA7vA-yp488In4wueKANCu_QaOPMhVJD_gxhE32g_GUgauTaUWOm0WAPWvLAP4iTnlLtjyF_4cH7PL8yyIloDnl6i8ZeFvLiEkCDU35OyjZZCtE1w3x47E14ETa_MOkPL9m--G6SjAok-lglPFc6Fxujw3qa1h-l60hTEWR-qKg_bIVZSud2_rilvHfVga_nGS9UI2JmSItL2JyHfpedtFbNZ7vYhPzYB54ODQKfjCG9_GBm7G1yrPO7ybw1hDWwgTIxYCAExBdif1ImiOb8D_w-4Ojs0dPB6eEVcLvaM6MoTRM43tasLwnc_kIdQAM4r9fAd16PTDXE5pZgA-HOtmnY3TRdfNJoQzoCIKmJAarTb0lCUwqgjrixBAGtuWBC7KOp-EX3dxSClI914Ju984z9a4cwXTOVRQIwHdLElkNTInW222f7mpBgbTkSmQBsfcxP-jaYxc1zUDZC-zu3P8h4UoDyFTB6WYR4jBm2ifWJAYl_ST1ya3Fd6ksfWpuJJXt3eW0nfzkDun8eSFm3MN37qZhRHyLJirS0PzHHrm5Vndcq70tg-hlQhQOnroL5F3KftlcEVMaonnyPkpJWpqEFwFuOKhVP66UkgV79MQ3h94ihZ7oPcBIlbsoMtb-haJ9wkGCK5JDZtfwumrduqQWbY18xXfHeViTIhOFg0jiDg1-GsFdQj_aaFNWS_dBx09EPKaAyBegGFE44xzDFxxn_QgeRD3J3R8M2ZyqbUDYMUetiJ8CsjxMk5rPTxHg8coroB5b7mpBCVFYvGvK0pHDOlVQ_=w1621-h933-s-no?authuser=0' width=\"600\" height=\"300\"></p>\n";
  ptr += "<p><img src='https://lh3.googleusercontent.com/pw/AJFCJaXsi81TvsGtEWy91GqAYGXRzy7nhLSrEDwMfvfo2EfViuPNYj07y47e9fD5hOp2I09Xgywa6hnO4c1IUopY26w2-hSUFWVOmw_cW8WXm2WDnKVj5O_1-S0j0fRGZ15HYrGPFHu_lDf3Ow_qo1eJsJMaXdlwoOEkhPC3olpL013VmonB0qt0Ar8EsEmTMRbKVexJq4ZhmorziIV2eBxwWNCUeZ0OL1yeQQ6gyjYzj6GAbeGk_EyDFQ0dbPQ8cxkRhtGOlXHGEbTyZRte7iyVgp_uhElaynu9mkAsSLS_q8TKawCpIiofswkhu6CMc8HXkzTOpqaOU1z6PRBnMms5N7aeSKMv2YPtL_tL6QaR_5vlAyNJS1IbdHAMfnloYHi4NLdy83vFPO6uKZt-x7TZhYU5af4-FlMz6dx7xnu535Mqw9zTTfMQEpnc_scIT2SAsB3y60hHZP7L9nij1vRU0ECnpsaj9i1qcNa8NZD0VGR_iXiiqpXkVqfQ_sKyO_EgDJsf9IIiVAr9EENotx5fjM7wvFwWZkl4pEWo0F2NuZvcYMkm2kNBNAfsABxeGH3cJ7u2034-Hs7YmRbGgJJjLFI3FNugtAPh7v5E2oMKK53Lk8ypULKPoXApM2awIZr6-gY7KTzyCVkL6F3J_P4D-uCJjpwBM3hFKEBVnF9aAUoEDbit1wGFMEDHncY4t1DLCaH_Q4h_-YgOMNp48rh5t9m8pFyE5blKJ-Hqr1e-vhWIVoiX8co-aEtQyx7dCkKpXyde2pfbwA9d73UKzWsT165RbkCTY-b-2PCWUihWQTekXzGpDwHAcjYqUpXscymnysugwFC8febkE6l3JdDvTYYjnRq9M81B6IYY5STR2FkrhJurOBrhS4lOrGgxhoAOEcbEbr_7emGz=w1918-h933-s-no?authuser=0' width=\"600\" height=\"300\"></p>\n";
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

  ptr += "<div id=\"leftl\">";
  ptr += "<fieldset><h4>Temperatura</h4></fieldset>\n";
  ptr += "<div id=\"leftl\">";
  ptr += "<a class=\"button button\" href=\"/Temp\">";
  ptr += temperatura;
  ptr += " C</a></div>\n";
  ptr += "</div>\n";

  ptr += "<div id=\"rightr\">\n";
  ptr += "<fieldset><h4>Puerta</h4></fieldset>\n";
  ptr += "<div id=\"leftl\">\n";
  if(EstadoPuerta)
  {
    ptr += "<a class=\"button button-on\" href=\"PuertaOff\">Abierta</a>";
  }
  else
  {
    ptr += "<a class=\"button button-off\" href=\"PuertaOn\">Cerrada</a>";
  }
  ptr +="</div>\n";
 
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

void handle_PuertaOn() {
  EstadoPuerta = HIGH;
  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura, true)); // 3
}

void handle_PuertaOff() {
  EstadoPuerta = LOW;
  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura, false)); // 3
}

void handle_Temp(){
  if(bmp280.takeForcedMeasurement()){
    temperatura=bmp280.readTemperature();
  }
  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura, EstadoPuerta)); // 3
}

void handle_OnConnect() {
  LEDA2Estado = LOW; // 1
  LEDA3Estado = LOW;
  LEDB1Estado = LOW;
  LEDB2Estado = LOW;
  LEDB3Estado = LOW;
  LedSectorAStat = LOW;
  LedSectorBStat = LOW;
  if(bmp280.takeForcedMeasurement()){
    temperatura = bmp280.readTemperature();
  }
  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura, EstadoPuerta)); // 3
}
 
void handle_LedSectorAon() {
  LEDA2Estado = HIGH; // 1
  LEDA3Estado = HIGH;
  LedSectorAStat = HIGH;
  if(bmp280.takeForcedMeasurement()){
    temperatura = bmp280.readTemperature();
  }
  server.send(200, "text/html", SendHTML(true, LedSectorBStat, targeta, temperatura, EstadoPuerta)); //3
}
 
void handle_LedSectorAoff() {
  LEDA2Estado = LOW; // 1
  LEDA3Estado = LOW;
  LedSectorAStat = LOW;
  if(bmp280.takeForcedMeasurement()){
    temperatura = bmp280.readTemperature();
  }
  server.send(200, "text/html", SendHTML(false, LedSectorBStat, targeta, temperatura, EstadoPuerta));
}
 
void handle_LedSectorBon() {
  LEDB1Estado = HIGH;
  LEDB2Estado = HIGH;
  LEDB3Estado = HIGH;
  LedSectorBStat = HIGH;
  if(bmp280.takeForcedMeasurement()){
    temperatura = bmp280.readTemperature();
  }
  server.send(200, "text/html", SendHTML(LedSectorAStat, true, targeta, temperatura, EstadoPuerta));
}
 
void handle_LedSectorBoff() {
  LEDB1Estado = LOW;
  LEDB2Estado = LOW;
  LEDB3Estado = LOW;
  LedSectorBStat = LOW;
  if(bmp280.takeForcedMeasurement()){
    temperatura = bmp280.readTemperature();
  }
  server.send(200, "text/html", SendHTML(LedSectorAStat, false, targeta, temperatura, EstadoPuerta));
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
* Configuracio Motor Porta
*/
servo.attach(26);
servo.write(angle);


/*
* Configuracion del Display
*/
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();

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
  server.on("/Temp", handle_Temp); //2
  server.on("/PuertaOn", handle_PuertaOn); //2
  server.on("/PuertaOff", handle_PuertaOff); //2
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
                  if(bmp280.takeForcedMeasurement()){
                    temperatura = bmp280.readTemperature();
                  }
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();
                  display.setTextSize(1);
                  display.setTextColor(WHITE);
                  display.setCursor(0,0);

                  display.print("\nUser: ");
                  if(targeta == "ab540d41"){
                    display.println("Pol");
                    for(angle; angle < 180; angle++){
                      servo.write(angle);
                    }
                    EstadoPuerta = HIGH;
                  }
                  else {
                    display.println("Unknown");
                  }
                  server.send(200, "text/html", SendHTML(LedSectorAStat, LedSectorBStat, targeta, temperatura, EstadoPuerta));
                  display.print("\nID: ");
                  display.println(targeta);

                  display.display();
                  display.clearDisplay();
            }      
	}

  if(EstadoPuerta) {
    for(angle; angle < 180; angle++){
      servo.write(angle);
    }
  }
  else{
    for(angle; angle > 60; angle--){
      servo.write(angle);
    }
  }

  if (LedSectorAStat)
  {
    digitalWrite(LEDA2pin, HIGH);
    digitalWrite(LEDA3pin, HIGH);
  }
  else
  {
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