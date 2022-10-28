
#if 1==2
/*  BLE_Peripheral.ino
*/
#endif

#include <ArduinoBLE.h>

const char* deviceServiceUuid = "19b1";
const char* deviceServiceCharacteristicUuid = "1214";

int data = -1;

BLEService dataTransferService(deviceServiceUuid); 
BLEByteCharacteristic dataTransferCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);


void setup() {
  Serial.begin(9600);
  while (!Serial); 
 
  
  if (!BLE.begin()) {
    Serial.println("- Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("Peripheral");
  BLE.setAdvertisedService(dataTransferService);
  dataTransferService.addCharacteristic(dataTransferCharacteristic);
  BLE.addService(dataTransferService);
  dataTransferCharacteristic.writeValue(-1);
  BLE.advertise();
  Serial.println("Peripheral Device");

}

void loop() {
  BLEDevice central = BLE.central();
  Serial.println("Discovering central ..... ...");
  delay(500);

  if (central) {
    Serial.println("Connected to central ");
    Serial.print("MAC :  ");
    Serial.println(central.address());

    while (central.connected()) {
      data=3;
      receiveData();
    }
    
    Serial.println("Disconnected to central device!");
  }
}


#if 1==3
/*
To send data 
*/

#endif

void sendData(){ 
  if(dataTransferCharacteristic.read()||dataTransferCharacteristic.written()){
    Serial.println("possible to write to central");
  } 
      while(data<=125)   { 
        Serial.print("data:  "); 
        Serial.println(data);    
        if (data%2 != 0) {      
            Serial.print(" Writing ............ ");
            Serial.println(data);
            dataTransferCharacteristic.writeValue((byte)data);
            // Serial.println("* Writing .......... done!");
            data+=1;
          }
        else {  
          data+=1;
          continue;
        }       
    }
    Serial.println("Data Transfer is done!");
    exit(1); 

}


#if 1==4
/*To receive data
*/
#endif

void receiveData(){

 if (dataTransferCharacteristic.written()) {
         data = (int)dataTransferCharacteristic.value();
         Serial.println(data);
               }


}
