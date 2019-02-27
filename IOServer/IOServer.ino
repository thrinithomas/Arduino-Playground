#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xE7, 0x65, 0xD5, 0x55, 0xBC, 0x76 }; byte ip[] = { 192, 168, 70, 247 };
byte gateway[] = { 192, 168, 70, 1 };
byte subnet[] = { 255, 255, 255, 0 };
int switchPower = 7;
int signalLight = 8;
int errorLight = 13;
const String sign = "LIGHTUP";

EthernetServer server = EthernetServer(11378);
EthernetClient clients[8];
String messageBuffer = "";

// non-block delay
unsigned long delayStart = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPower, OUTPUT);
  pinMode(errorLight, OUTPUT);
  pinMode(signalLight, OUTPUT);

  // Supply Power to Switch
  digitalWrite(switchPower, HIGH);
  
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);

  if(Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield missing");
    while(true) {
      digitalWrite(errorLight, HIGH);
      delay(1000);
      digitalWrite(errorLight, LOW);
      delay(1000);
    }
  }

  if(Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    while(true) {
      digitalWrite(errorLight, HIGH);
      delay(3000);
      digitalWrite(errorLight, LOW);
      delay(1000);
    }
  }

  delayStart = millis();

  server.begin();
  Serial.println("Server Start...");
  Serial.print("Server address:");
  Serial.println(Ethernet.localIP());

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
  
  EthernetClient newClient = server.accept();
   if(newClient) {
      for(byte i=0; i<8; i++) {
        if(!clients[i]){
          clients[i] = newClient;
          break;
        }
      }
    }

/*
  for(byte i=0; i<8; i++) {
    while(clients[i] && clients[i].available() >0) {
      Serial.write(clients[i].read());
      server.write(clients[i].read());
    }
  }
  */
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
