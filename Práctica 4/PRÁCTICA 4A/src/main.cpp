#include <WiFi.h>
#include <WebServer.h>
#include <String>

// SSID & Password
const char* ssid = "MiFibra-BC0E"; // Enter your SSID here
const char* password = "hArN674S"; //Enter your Password here
WebServer server(80); // Object of WebServer(HTTP port, 80 is defult)

String HTML = "";

//HTML Block
void html_block() {
    HTML += "<!DOCTYPE html>\n";
    HTML += "<html>\n";
    HTML += "<head>\n";
    HTML += "<meta charset='UTF-8'>\n";
    HTML += "<title>PR&Agrave;CTICA 4</title>\n";
    HTML += "<style>\n";
    //HTML += ".vscode-dark img[src$=\#gh-light-mode-only],\n";
    //HTML += ".vscode-light img[src$=\#gh-dark-mode-only] {\n";
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
    HTML += "</body>\n";
    HTML += "</html>";
}
// Handle root url 
void handle_root() {
    server.send(200, "text/html", HTML);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Try Connecting to ");
    Serial.println(ssid);
    // Connect to your wi-fi modem
    WiFi.begin(ssid, password);
    // Check wi-fi is connected to wi-fi network
    while (WiFi.status() != WL_CONNECTED) {
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
    delay(100);
}
void loop() {
    server.handleClient();
}
