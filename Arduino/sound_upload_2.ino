#include <ArduinoBLE.h> 
#include <PDM.h>

const char* nameOfPeripheral = "MicrophoneMonitor"; 
const char* uuidOfService = "00001101-0000-1000-8000-00805f9b34fb"; 
const char* uuidOfRxChar = "00001142-0000-1000-8000-00805f9b34fb"; 
const char* uuidOfTxChar = "00001143-0000-1000-8000-00805f9b34fb";

BLEService microphoneService(uuidOfService);

const int WRITE_BUFFER_SIZE = 256; 
bool WRITE_BUFFER_FIZED_LENGTH = false;

BLECharacteristic rxChar(uuidOfRxChar, BLEWriteWithoutResponse | BLEWrite, WRITE_BUFFER_SIZE, WRITE_BUFFER_FIZED_LENGTH); 
BLEByteCharacteristic txChar(uuidOfTxChar, BLERead | BLENotify | BLEBroadcast);

unsigned long times;

bool delayOk = false;

short sampleBuffer[256];

volatile int samplesRead;

void setup() { // Start serial. 
    Serial.begin(116200);
    while (!Serial);
    
    pinMode(LED_BUILTIN, OUTPUT); 
    pinMode(LEDR, OUTPUT); 
    pinMode(LEDG, OUTPUT);

    PDM.onReceive(onPDMdata);

    startPDM();

    startBLE();

    BLE.setLocalName(nameOfPeripheral); 
    BLE.setAdvertisedService(microphoneService);

    microphoneService.addCharacteristic(rxChar); 
    microphoneService.addCharacteristic(txChar);

    BLE.addService(microphoneService);

    BLE.setEventHandler(BLEConnected, onBLEConnected); 
    BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);

    rxChar.setEventHandler(BLEWritten, onRxCharValueUpdate);

    BLE.advertise();

    Serial.println("Peripheral advertising info: "); 
    Serial.print("Name: "); 
    Serial.println(nameOfPeripheral); 
    Serial.print("MAC: "); 
    Serial.println(BLE.address()); 
    Serial.print("Service UUID: "); 
    Serial.println(microphoneService.uuid()); 
    Serial.print("rxCharacteristic UUID: "); 
    Serial.println(uuidOfRxChar); 
    Serial.print("txCharacteristics UUID: "); 
    Serial.println(uuidOfTxChar);

    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
    BLEDevice central = BLE.central();
    times = millis();
    
    if (central)
    {
        while (central.connected()) 
        { 
            connectedLight();

            if(!delayOk && (times > 2000)){
              
                 if (samplesRead) {
                    for (int i = 0; i < samplesRead; i++) 
                    { 
                        txChar.writeValue(sampleBuffer[i]);
                    }
                    samplesRead = 0;
                 }
                 times = 0;
            }
            
        }
        disconnectedLight();
    } else {
        disconnectedLight();
    }
}

void startBLE() { 
    if (!BLE.begin()) { 
        Serial.println("starting BLE failed!"); 
        while (1); 
    }
}

void onRxCharValueUpdate(BLEDevice central, BLECharacteristic characteristic) {
    Serial.print("Characteristic event, read: "); 
    byte test[256]; 
    int dataLength = rxChar.readValue(test, 256);

    for(int i = 0; i < dataLength; i++) { 
        Serial.print((char)test[i]); 
    }
    Serial.println(); 
    Serial.print("Value length = "); 
    Serial.println(rxChar.valueLength());
}

void onBLEConnected(BLEDevice central) { 
    Serial.print("Connected event, central: "); 
    Serial.println(central.address()); 
    connectedLight(); 
}

void onBLEDisconnected(BLEDevice central) { 
    Serial.print("Disconnected event, central: "); 
    Serial.println(central.address()); 
    disconnectedLight(); 
}

void startPDM() { 
    if (!PDM.begin(1, 22050)) { 
        Serial.println("Failed to start PDM!"); 
        while (1); 
    } 
}

void onPDMdata() { 
    int bytesAvailable = PDM.available(); 
 
    PDM.read(sampleBuffer, bytesAvailable); 

    samplesRead = bytesAvailable / 2; 
}

void connectedLight() { 
    digitalWrite(LEDR, LOW); 
    digitalWrite(LEDG, HIGH); 
}

void disconnectedLight() { 
    digitalWrite(LEDR, HIGH); 
    digitalWrite(LEDG, LOW); 
}