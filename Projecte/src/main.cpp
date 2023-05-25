/*
* Servidor web con ESP32
* Librerias necesarias para conectarnos a un entorno Wifi y poder configurar  
* un servidor WEB
*/
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>

/*
  Pins para el lector RFID
*/
#define RST_PIN	26    //Pin 9 para el reset del RC522
#define SS_PIN	21   //Pin 10 para el SS (SDA) del RC522
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
uint8_t LED1pin = 12;
bool LED1Estado = LOW;
uint8_t LED2pin = 14;
bool LED2Estado = LOW;
uint8_t LED3pin = 27;
bool LED3Estado = LOW;
 
/*
   Aqui esta definido todo el HTML y el CSS del servidor WEB con ESP32
*/
String SendHTML(uint8_t led1stat, uint8_t led2stat, uint8_t led3stat) {
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
  ptr += ".button {display: block;width: 80px;text-align: center;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {text-align: center;font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  /*
   * Encabezados de la pagina
   */
  ptr += "<h1 align=\"center\">ESP32 Servidor WEB</h1>\n";
  ptr += "<h3 align=\"center\">Usando Modo Estacion</h3>\n";
/*
 * Aqui esta la inteligencia del servidor WEB con ESP32, dependiento de los parametros de la funcion SendHTML
 * modificara la vista de la pagina WEB,  llamaran a las clases "button-on y button-off" que cambian como
 * se muestran los datos en la pagina WEB 
 */
  if (led1stat)
  {
    ptr += "<p>LED1 Estado: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>";
  }
  else
  {
    ptr += "<p>LED1 Estado: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>";
  }
 
  if (led2stat)
  {
    ptr += "<p>LED2 Estado: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>";
  }
  else
  {
    ptr += "<p>LED2 Estado: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>";
  }
 
  if (led3stat)
  {
    ptr += "<p>LED3 Estado: ON</p><a class=\"button button-off\" href=\"/led3off\">OFF</a>";
  }
  else
  {
    ptr += "<p>LED3 Estado: OFF</p><a class=\"button button-on\" href=\"/led3on\">ON</a>";
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
void handle_OnConnect() {
  LED1Estado = LOW; // 1
  LED2Estado = LOW; // 1
  LED3Estado = LOW; // 1
  Serial.println("GPIO4 Estado: OFF | GPIO5 Estado: OFF"); // 2
  server.send(200, "text/html", SendHTML(LED1Estado, LED2Estado, LED3Estado)); // 3
}
 
void handle_led1on() {
  LED1Estado = HIGH; //1
  Serial.println("GPIO4 Estado: ON"); // 2
  server.send(200, "text/html", SendHTML(true, LED2Estado, LED3Estado)); //3
}
 
void handle_led1off() {
  LED1Estado = LOW;
  Serial.println("GPIO4 Estado: OFF");
  server.send(200, "text/html", SendHTML(false, LED2Estado, LED3Estado));
}
 
void handle_led2on() {
  LED2Estado = HIGH;
  Serial.println("GPIO5 Estado: ON");
  server.send(200, "text/html", SendHTML(LED1Estado, true, LED3Estado));
}
 
void handle_led2off() {
  LED2Estado = LOW;
  Serial.println("GPIO5 Estado: OFF");
  server.send(200, "text/html", SendHTML(LED1Estado, false, LED3Estado));
}

void handle_led3on() {
  LED3Estado = HIGH;
  Serial.println("LED3 Estado: ON");
  server.send(200, "text/html", SendHTML(LED1Estado, LED2Estado, true));
}
 
void handle_led3off() {
  LED3Estado = LOW;
  Serial.println("GPIO5 Estado: OFF");
  server.send(200, "text/html", SendHTML(LED1Estado, LED2Estado, false));
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
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
/*
* BUS SPI del RFID
*/
  SPI.begin();
  mfrc522.PCD_Init();
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
  server.on("/led1on", handle_led1on); // 2
  server.on("/led1off", handle_led1off); // 2
  server.on("/led2on", handle_led2on); // 2
  server.on("/led2off", handle_led2off); // 2
  server.on("/led3on", handle_led3on); // 2
  server.on("/led3off", handle_led3off);// 2
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
                  // Enviamos serialemente su UID
                  Serial.print("Card UID:");
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);
                          targeta[i]=(mfrc522.uid.uidByte[i] < 0x10 ? '0' : ' ');
                          targeta[i]=mfrc522.uid.uidByte[i],HEX;
                          
                  } 
                  Serial.println();
                  Serial.print("Carda v2:");
                  Serial.print(targeta);
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();         
            }      
	}


  if (LED1Estado)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }
 
  if (LED2Estado)
  {
    digitalWrite(LED2pin, HIGH);
  }
  else
  {
    digitalWrite(LED2pin, LOW);
  }

  if (LED3Estado)
  {
    digitalWrite(LED3pin, HIGH);
  }
  else
  {
    digitalWrite(LED3pin, LOW);
  }
}