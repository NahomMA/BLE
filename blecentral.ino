/*
  BLE_Central_Device.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE Sense 
  as a central device and looks for a specified service and characteristic in a 
  peripheral device. If the specified service and characteristic is found in a 
  peripheral device, the last detected value of the on-board gesture sensor of 
  the Nano 33 BLE Sense, the APDS9960, is written in the specified characteristic. 

  The circuit:
  - Arduino Nano 33 BLE Sense. 

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>

const char* deviceServiceUuid = "19b1";
const char* deviceServiceCharacteristicUuid = "1214";

int flag=0, data = -1;

int *tmp= &data;

int oldData = 1;   

void setup() {
  Serial.begin(9600);
  while (!Serial); 

  if (!BLE.begin()) {
    Serial.println("* Starting BLE module failed!");
    while (1);
  }
  
  BLE.setLocalName("CentralBLE"); 
  BLE.advertise();
  Serial.println("Central Device");
 
}

void loop() {
  connectToPeripheral();
}

void connectToPeripheral(){
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral........");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("Connecting to peripheral...........");

  if (peripheral.connect()) {
    Serial.println("Connected to peripheral ");
    Serial.println(" ");
  } else {
    Serial.println("Connection failed!");  
    return;
  }


  Serial.println("Discovering attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Peripheral attributes discovered!");
  
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
 
    peripheral.disconnect();
    return;
  }

  BLECharacteristic dataTransferCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
  if (!dataTransferCharacteristic) {
    Serial.println("NO data type charx!");
    peripheral.disconnect();
    return;
  } else if (!dataTransferCharacteristic.canWrite() ) {
    Serial.println("no writable from peripheral!");
    peripheral.disconnect();
    return;
  }  
  
  while (peripheral.connected()) {    
      data = 3;      
      while(data<=125)   {      
        if (data%2 != 0) {      
            Serial.print("* Writing ............ ");
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
  Serial.println("- Peripheral device disconnected!");
}




