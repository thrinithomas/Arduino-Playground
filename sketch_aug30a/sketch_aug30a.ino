#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "thrini-scitechchallenge2018.firebaseio.com"
#define FIREBASE_AUTH "-------FIREBASE_AUTH--------"

#define WIFI_SSID "Elderly"
#define WIFI_PASSWORD "--------PASSWORD----------"

#define LED 5
#define MAGNET 4
int val = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(MAGNET, INPUT);
  // digitalWrite(LED, 0);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, 0);
    Serial.print(".");
    delay(500);
    digitalWrite(LED, 1);
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Firebase.setInt("DoorStatus", 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if(Firebase.getInt("LEDStatus")) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  */
  // digitalRead(LED);
  val = digitalRead(MAGNET);
  Serial.print("Magnet State: "); Serial.println(val); 
  digitalWrite(LED, val);
  /*
  if(val == 0) {
    digitalWrite(LED, 1);  
  }else{
    digitalWrite(LED, 0);
  }
  */
  Firebase.setInt("DoorStatus", val);
  delay(2500);
  
  if(Firebase.failed()) {
    Serial.print("setting/number failed: ");
    Serial.println(Firebase.error());
    return;
  }
}
