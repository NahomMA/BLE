/*
  BLE_Peripheral.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE 
  as a peripheral device and specifies a service and a characteristic. Depending 
  of the value of the specified characteristic, an on-board LED gets on. 

  The circuit:
  - Arduino Nano 33 BLE. 

  This example code is in the public domain.
*/

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
      if (dataTransferCharacteristic.written()) {
         data = (int)dataTransferCharacteristic.value();
         Serial.println(data);
               }
    }
    
    Serial.println("Disconnected to central device!");
  }
}


