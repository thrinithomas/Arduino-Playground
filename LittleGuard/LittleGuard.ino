// I/O Defination
int supLight = 13; // Suspension Light
int supButton = 12; // Suspension Button
int irLight = 7; // IR Alert Light/Buzzer
int irTrigger = 6; // IR Alter Trigger
int cancel = 4; // Cancel Alert

// Millis roll-over
int interval = 300; // 0.5 second
unsigned long cacheMillis=0;
unsigned long delayStart = 0; // Global Locker, semiphrone
int delayTimer = 5000; // Delay Time Defination

void setup() {
  // put your setup code here, to run once:
  pinMode(supLight, OUTPUT); // supLight HIGH = functioning, LOW = suspended
  pinMode(supButton, INPUT);
  pinMode(irLight, OUTPUT);
  pinMode(irTrigger, INPUT);
  pinMode(cancel, INPUT);

  Serial.begin(9600); // Logger
  delayStart = millis();

  
  while(millis() - delayStart < delayTimer) {
    unsigned long currentMillis = millis();
    if((unsigned long)(currentMillis - cacheMillis) >= interval){
      // Flash 2 signal light
      if((unsigned long)((currentMillis - cacheMillis / interval)% 2 == 0)) {
       digitalWrite(supLight, HIGH);
       digitalWrite(irLight, HIGH);
      } else {
       digitalWrite(supLight, LOW);
       digitalWrite(irLight, LOW); 
      }
     cacheMillis = currentMillis;
      // Serial.print("LED Signal: supLight -> "); Serial.println(digitalRead(supLight));
      // Serial.print("LED Signal: irLight -> "); Serial.println(digitalRead(irLight));
    }
  }

  // Pull down 2 signal light - Start Program
  digitalWrite(supLight, HIGH);
  digitalWrite(irLight, LOW);
  interval = 1000;
}

void loop() {
  // put your main code here, to run repeatedly:

  // Avoid millis roll-over, use currentMillis
  unsigned long currentMillis = millis();
  if((unsigned long)(currentMillis - cacheMillis) >= interval) {
    // digitalWrite(13, !digitalRead(13));
    cacheMillis = currentMillis;
  }

  // Ststus Check
  Serial.println("/---- Status ----/");
  Serial.print("Suspension "); Serial.println(digitalRead(supButton));
  Serial.print("Pulse "); Serial.println(pulseIn(supButton, HIGH));
  Serial.println();

  if((cacheMillis - delayStart) >= delayTimer) {
    // -- Functioning --
    // Reset the suspension light after suspension end
    if(digitalRead(supLight) == LOW) {
      digitalWrite(supLight, HIGH);
    }
    
    Serial.println("System fucnctioning");

    // The nature of button make it need to read press pulse
    if(digitalRead(supButton) == HIGH && pulseIn(supButton, HIGH) == 0) { 
      Serial.println("Suspension Button Pressed");
      delayStart = millis();
      delayTimer = 30000; // Update delay timer to 30s
      digitalWrite(supLight, LOW); // Dim suspension light
      digitalWrite(irLight, LOW); // Dim Alert Light
    }

    // Check if anything by pass IR sensor
    if(digitalRead(irTrigger) == HIGH) {
      digitalWrite(irLight, HIGH);
    }

    // Cancel Button, reset itLight
    if(digitalRead(cancel) == HIGH) {
      digitalWrite(irLight, LOW);
    }
    
  } else {
    // -- Suspension --
    Serial.println("System suspension");
  }

  Serial.print("System Running Time: "); Serial.println(cacheMillis / 1000);
  Serial.println();
  delay(1000);
}
