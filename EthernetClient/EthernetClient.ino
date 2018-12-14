#include<SPI.h>
#include<Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// IPAddress ip(192, 168, 1, 21);
byte server[] = {192, 168, 1, 13};
EthernetClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(Ethernet.begin(mac)==0){
    Serial.println("Can't get IP");
    for(;;)
      ;
  }
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Connecting....");

}

void loop() {
  if(client.connect(server, 59998)){
    Serial.println("--> Connected");
    client.print("BA");
  } else {
    Serial.println("--> Connection Failed");
    client.flush();
    client.stop();
  }
  delay(10000);
}
