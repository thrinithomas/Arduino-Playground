#include <SPI.h>
#include <Ethernet.h>

// The network config.
byte mac[] = { 0xE7, 0x65, 0xD5, 0x55, 0xBC, 0x76 };
byte ip[] = { 192, 168, 1, 19 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };

// Ethernet Server config.
EthernetServer server = EthernetServer(11378);
EthernetClient clients[8];

// IO Pin config.
int gpo7 = 7;
int gpo8 = 8;
int gpo9 = 9;
int gpo10 = 10;
int gpo11 = 11;
int errorLight = 13;

String messageBuffer = "";

// non-block delay
unsigned long delayStart = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(gpo7, OUTPUT);
  pinMode(gpo8, OUTPUT);
  pinMode(gpo9, OUTPUT);
  pinMode(gpo10, OUTPUT);
  pinMode(gpo11, OUTPUT);

  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield missing");
    while (true) {
      digitalWrite(errorLight, HIGH);
      delay(1000);
      digitalWrite(errorLight, LOW);
      delay(1000);
    }
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    while (true) {
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
  // This logic is to avoid any disconnect from client program
  // that makes IO port always ON
  unsigned long currentMillis = millis();
  if (((unsigned long)(currentMillis - delayStart) >= 10000)){ // && (digitalRead(signalLight) == HIGH)) {
    resetOutput();
    delayStart = millis(); // Reset Delay Start
    Serial.println("IO Timeout");
  } else {
    //int nonTime = (millis() - delayStart);
    //Serial.println("Debug: " + nonTime);
  }

  EthernetClient newClient = server.accept();
  if (newClient) {
    for (byte i = 0; i < 8; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        break;
      }
    }
  }

  for (byte i = 0; i < 8; i++) {
    if (clients[i].available() > 0) {
      char thisChar = clients[i].read();
      if (thisChar == '\n') {
        Serial.println(messageBuffer);
        handleClientMsg(messageBuffer);
        delayStart = millis();
        messageBuffer = "";
      } else {
        messageBuffer += thisChar;
      }
    }
  }
}

void handleClientMsg(String msg){
  // Why if-else chain?
  // because C don't support string switch
  // can construct a Key-Value type by using struct 
  if(msg == "ON7") {
    digitalWrite(gpo7, HIGH);
    return;
  } else if(msg == "OFF7") {
    digitalWrite(gpo7, LOW);
    return;
  } else if(msg == "ON8") {
    digitalWrite(gpo8, HIGH);
    return;  
  } else if(msg == "OFF8") {
    digitalWrite(gpo8, LOW);
    return;
  } else if(msg == "ON9") {
    digitalWrite(gpo9, HIGH);
    return;
  } else if(msg == "OFF9") {
    digitalWrite(gpo9, LOW);
    return;  
  } else if(msg == "ON10") {
    digitalWrite(gpo10, HIGH);
    return;  
  } else if(msg == "OFF10") {
    digitalWrite(gpo10, LOW);
    return;
  } else if (msg == "ON11") {
    digitalWrite(gpo11, HIGH);
    return;
  } else if (msg == "OFF11") {
    digitalWrite(gpo11, LOW);
    return;
  } else {
    return;
  }
}

void resetOutput() {
  digitalWrite(gpo7, LOW);
  digitalWrite(gpo8, LOW);
  digitalWrite(gpo9, LOW);
  digitalWrite(gpo10, LOW);
  digitalWrite(gpo11, LOW);
}
