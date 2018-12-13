#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xAB, 0xAA, 0x30, 0x20, 0x13, 0x33 };
byte ip[] = { 192,168,1,20 };
byte gateway[] = { 192,168,1,1 };
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer server = EthernetServer(59998);

void setup() {
  // put your setup code here, to run once:
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin();
  Serial.println("Server Start...");
  Serial.print("Server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  EthernetClient client = server.available();
  if(client) {
    if(client.available() > 0) {
      char thisChar = client.read();
      Serial.print(thisChar, HEX);
      Serial.print(" Equivalent to ");
      Serial.println(thisChar, BIN);
      // Response Logic here
    }
  }
}
