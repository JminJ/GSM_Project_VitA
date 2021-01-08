#include <ArduinoBLE.h> 
#include <PDM.h>

const char* nameOfPeripheral = "MicrophoneMonitor"; 
const char* uuidOfService = "00001101-0000-1000-8000-00805f9b34fb"; 
const char* uuidOfRxChar = "00001142-0000-1000-8000-00805f9b34fb"; 
const char* uuidOfTxChar = "00001143-0000-1000-8000-00805f9b34fb";

BLEService microphoneService(uuidOfService);

const int WRITE_BUFFER_SIZE = 256; 
bool WRITE_BUFFER_FIZED_LENGTH = false;

BLEStringCharacteristic rxChar(uuidOfRxChar, BLEWriteWithoutResponse | BLEWrite, 10); 
BLEByteCharacteristic txChar(uuidOfTxChar, BLERead | BLENotify | BLEBroadcast);

unsigned long times;

bool delayOk = false;
const int VibrationPin = 3;

short sampleBuffer[256];

volatile int samplesRead;

char result = '0';


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
            }else{
                if(delayOk){
                    for(int i = 0; i < 2; i++){
                        vibration(result);
                    }
                }else{
                    vibration(result);
                }
                delayOk = false;
            }
            times = 0;       
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
    /*byte test[256];*/ 
    //int dataLength = rxChar.readValue(test, 256);

    if(rxChar.written()) {
        if (rxChar.value()) {
            result = rxChar.value();

            Serial.print("Class: ");
            Serial.println(result);
        }//value
    }//written    

    /*for(int i = 0; i < dataLength; i++) { 
        Serial.print((char)test[i]); 
    }
    Serial.println(); 
    Serial.print("Value length = "); 
    Serial.println(rxChar.valueLength());*/
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

void vibration(char classNum){

    char arr[7] = {'1','2','3','4','5','8','9'};

    for(int i = 0; i < 7; i++){
        if(arr[i] == classNum){
            delayOk = true;
        }
    }

    for(int i = 0; i < 2; i++){
        connection(result);
    }
    delayOk = false;
}

void connection(char result){
    switch (result)
    {
    case '1':
        vibratoin_1();
        break;
    case '2':
        vibratoin_2();
        break;
    case '3':
        vibratoin_3();
        break;
    case '4':
        vibratoin_4();
        break;
    case '5':
        vibratoin_5();
        break;
    case '8':
        vibratoin_8();
        break;
    case '9':
        vibratoin_9();
        break;
    default:
        break;
    }
}

void vibratoin_1(){
    analogWrite(VibrationPin, 255);
    delay(150);
    analogWrite(VibrationPin, 200);
    delay(150);
    analogWrite(VibrationPin, 150);
    delay(250);
    analogWrite(VibrationPin, 100);
    delay(250);
    analogWrite(VibrationPin, 0);
}

void vibratoin_2(){
    analogWrite(VibrationPin, 70);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(500);

    analogWrite(VibrationPin, 200);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(500);

    analogWrite(VibrationPin, 70);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(500);
}

void vibratoin_3(){
    analogWrite(VibrationPin, 70);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(500);

    analogWrite(VibrationPin, 200);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(500);

    analogWrite(VibrationPin, 70);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(500);
}

void vibratoin_4(){
    analogWrite(VibrationPin, 130);
    delay(250);
    analogWrite(VibrationPin, 160);
    delay(250);
    analogWrite(VibrationPin, 200);
    delay(250);
    analogWrite(VibrationPin, 130);
    delay(250);
    analogWrite(VibrationPin, 0);
}

void vibratoin_5(){
    analogWrite(VibrationPin, 80);
    delay(150);
    analogWrite(VibrationPin, 160);
    delay(100);
    analogWrite(VibrationPin, 255);
    delay(250);
    analogWrite(VibrationPin, 110);
    delay(100);
    analogWrite(VibrationPin, 190);
    delay(150);
    analogWrite(VibrationPin, 255);
    delay(250);
    analogWrite(VibrationPin, 0);
}

void vibratoin_8(){ // siren
    analogWrite(VibrationPin, 200);
    delay(1000);
    analogWrite(VibrationPin, 0);
    delay(600);

    analogWrite(VibrationPin, 200);
    delay(1000);
    analogWrite(VibrationPin, 0);
    delay(600);
}

void vibratoin_9(){ // street_music
    analogWrite(VibrationPin, 200);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(200);
    analogWrite(VibrationPin, 200);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(200);
    analogWrite(VibrationPin, 200);
    delay(300);
    analogWrite(VibrationPin, 0);
    delay(200);

    analogWrite(VibrationPin, 70);
    delay(500);
    analogWrite(VibrationPin, 0);
    delay(70);
    analogWrite(VibrationPin, 200);
    delay(200);
    analogWrite(VibrationPin, 0);
    delay(70);
    analogWrite(VibrationPin, 70);
    delay(500);
}
