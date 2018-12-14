//COM8
#include "SoftwareSerial.h"
int rxPin = D1;
int txPin = D2;
SoftwareSerial swSerRemote(rxPin, txPin, true, 256);

bool initsend = false;
int counter = 1;

bool sendDataRemote = false;
unsigned long timerRemote = 0;
int packageCounterRemote = 0;
int packageRemote1 = 0;
int packageRemote2 = 0;
int packageRemote3 = 0;
int packageRemote4 = 0;

void setup() {  
  Serial.begin(115200);
  Serial.println("Starting Remote ....");
  swSerRemote.begin(960);
}

void loop() {

  if(!initsend){
    initsend = true;
    Serial.println("Sending Data To HeatPump");
    swSerRemote.write(170);
    swSerRemote.write(144);
    swSerRemote.write(17);
    swSerRemote.write(1);
  }  
  
  if(sendDataRemote){
    counter++;
    if(counter > 100){
      counter = 1;
    }
    sendDataRemote = false;
    delay(50);
    Serial.println("Sending Data To HeatPump");
    swSerRemote.write(170);
    swSerRemote.write(144);
    swSerRemote.write(17);
    swSerRemote.write(counter);    
  }    

  while (swSerRemote.available() > 0) {    
    unsigned int packagedata = swSerRemote.read();
    if(millis() - timerRemote > 30){
      packageCounterRemote = 1;
    }
    if(packageCounterRemote == 1 && packagedata != 85){
      Serial.print("Wrong Package Received from HeatPump");
      Serial.println(packagedata);  
      timerRemote = millis();
      continue;
    }
    Serial.print("Correct Package Received from HeatPump");
    Serial.println(packagedata);         
    if(packageCounterRemote==1){
      packageRemote1 = packagedata;
    }
    if(packageCounterRemote==2){
      packageRemote2 = packagedata;
    }    
    if(packageCounterRemote==3){
      packageRemote3 = packagedata;
    }  
    if(packageCounterRemote==4){
      packageRemote4 = packagedata;
      sendDataRemote = true;
      Serial.println("Complete Package received");
    }         
    timerRemote = millis();    
    packageCounterRemote++;
    yield();
  }  

}
