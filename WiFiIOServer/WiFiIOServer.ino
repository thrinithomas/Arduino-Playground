#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Constant
const char* ssid = "Elderly";
const char* password = "**********";
IPAddress ip(192, 168, 1, 239);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(11378);
WiFiClient clients[8];
String messageBuffer = "";

int switchPower = 13;
int signalLight = 0;
const String sign = "LIGHTUP";

// non-block delay
unsigned long delayStart = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switchPower, OUTPUT);
  digitalWrite(switchPower, HIGH);
  
  pinMode(signalLight, OUTPUT);
  Serial.begin(115200);
  
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  delayStart = millis();

  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // put your main code here, to run repeatedly:

  // This logic is to avoid any disconnect from client program
  // that makes IO port always ON
  if(((millis() - delayStart) >= 10000) && (digitalRead(signalLight) == HIGH)) {
    digitalWrite(signalLight, LOW);
    delayStart = millis(); // Reset Delay Start
    Serial.println("IO Timeout");
  } else {
    //int nonTime = (millis() - delayStart);
    //Serial.println("Debug: " + nonTime);
  }

  WiFiClient newClient = server.available();
  if(newClient){
    for(byte i=0; i<8; i++) {
      if(!clients[i]){
        clients[i] = newClient;
        break;
      }
    }
  }

    for(byte i=0; i<8; i++) {
     if(clients[i].available() > 0) {
      char thisChar = clients[i].read();
      if(thisChar == '\n') {
        Serial.println(messageBuffer);
        if(messageBuffer == sign) {
          digitalWrite(signalLight, HIGH);
        } else {
          digitalWrite(signalLight, LOW);
        }
        delayStart = millis();
        messageBuffer = "";
      } else {
        messageBuffer += thisChar;
      }
    } 
  }
}
