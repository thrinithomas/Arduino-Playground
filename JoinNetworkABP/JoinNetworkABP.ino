

#include "RAK811.h"
#include "SoftwareSerial.h"
#define WORK_MODE LoRaWAN   //  LoRaWAN or LoRaP2P
#define JOIN_MODE ABP    //  OTAA or ABP
#if JOIN_MODE == OTAA
String DevEui = "60C5A8FFFE000001";
String AppEui = "70B3D57EF00047C0";
String AppKey = "5D833B4696D5E01E2F8DC880E30BA5FE";
#else JOIN_MODE == ABP
String NwkSKey = "B9B3C3DE4203C2CC076D590B9DEE2EDD";
String AppSKey = "C757625DC35FCAA425E00A6544383AAA";
String DevAddr = "26011A12";
#endif
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define ATSerial Serial
SoftwareSerial DebugSerial(RXpin,TXpin);    // Declare a virtual serial port
char* buffer = "72616B776972656C657373";

bool InitLoRaWAN(void);
RAK811 RAKLoRa(ATSerial,DebugSerial);


void setup() {
 DebugSerial.begin(9600);
 while(DebugSerial.read()>= 0) {}
 while(!DebugSerial);
 DebugSerial.println("StartUP");

 ATSerial.begin(9600); // Note: Please manually set the baud rate of the WisNode device to 9600.
 delay(100);
 DebugSerial.println(RAKLoRa.rk_getVersion());
 delay(200);
 DebugSerial.println(RAKLoRa.rk_getBand());
 delay(200);

 while (!InitLoRaWAN());

 // LED Indicator
 pinMode(13, OUTPUT);
 pinMode(9, OUTPUT);

}

bool InitLoRaWAN(void)
{
  if (RAKLoRa.rk_setWorkingMode(WORK_MODE))
  {
    if (RAKLoRa.rk_initABP(DevAddr, NwkSKey, AppSKey))
    {
      DebugSerial.println("You init ABP parameter is OK!");
      if (RAKLoRa.rk_joinLoRaNetwork(JOIN_MODE))
      {
        DebugSerial.println("You join Network success!");
        return true;
      }
    }
  }
  return false;
}

void loop() {
  int packetsflag = 1; // 0: unconfirmed packets, 1: confirmed packets
  if (RAKLoRa.rk_sendData(packetsflag, 1, buffer))
  {
    for (unsigned long start = millis(); millis() - start < 90000L;)
    {
      digitalWrite(13, HIGH);
      delay(1000);
      String ret = RAKLoRa.rk_recvData();
      if (ret.startsWith(STATUS_TX_COMFIRMED) || ret.startsWith(STATUS_TX_UNCOMFIRMED))
      {
        DebugSerial.println("Send data ok!");
        digitalWrite(13, LOW);
        delay(2000);
        return;
      }
    }
    
    DebugSerial.println("Send data error!");
    while(1){
      digitalWrite(9, HIGH); // Raise the RED Signal Light
      delay(1000);
      digitalWrite(9, LOW);
    };
  }
}

