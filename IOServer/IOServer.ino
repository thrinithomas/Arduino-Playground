#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xAB, 0xAA, 0x30, 0x20, 0x13, 0x3A };
byte ip[] = { 192, 168, 1, 10 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 192, 168, 1, 111 };
int errorLight = 7;
int signalLight = 8;
const String sign = "LIGHTUP";

EthernetServer server = EthernetServer(11378);
EthernetClient clients[8];
String messageBuffer = "";

void setup() {
  // put your setup code here, to run once:
  pinMode(errorLight, OUTPUT);
  pinMode(signalLight, OUTPUT);
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

  server.begin();
  Serial.println("Server Start...");
  Serial.print("Server address:");
  Serial.println(Ethernet.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  
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
        messageBuffer = "";
      } else {
        messageBuffer += thisChar;
      }
  } 
  }
}
