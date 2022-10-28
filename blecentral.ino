#if 0
/* BLE_Central_Device.ino
*/
#endif

#include <ArduinoBLE.h>

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
  
  
  while (peripheral.connected()) {    
      data = 3; 
      sendData(peripheral,dataTransferCharacteristic);
  
  }
  Serial.println("- Peripheral device disconnected!");
}


#if 1==3
/*
To send data 
*/

#endif


void sendData(BLEDevice peripheral,BLECharacteristic dataTransferCharacteristic){
   if (!dataTransferCharacteristic) {
    Serial.println("NO data type charx!");
    peripheral.disconnect();
    return;
  } else if (!dataTransferCharacteristic.canWrite() ) {
    Serial.println("no writable from peripheral!");
    peripheral.disconnect();
    return;
  } 

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



#if 1==4
/*To receive data
*/

#endif
void receiveData(BLEDevice peripheral,BLECharacteristic dataTransferCharacteristic){
  if (!dataTransferCharacteristic) {
    Serial.println("NO data type charx!");
    peripheral.disconnect();
    return;
  } else if (!dataTransferCharacteristic.canWrite() ) {
    Serial.println("no readable from peripheral!");
    peripheral.disconnect();
    return;
  }
 if (dataTransferCharacteristic.written()) {
         data = (int)dataTransferCharacteristic.value();
         Serial.println(data);
               }


}
