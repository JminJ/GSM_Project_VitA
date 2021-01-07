#include <Arduino.h>

const int VibrationPin = 3;
bool delayOk = false;
// 바꾸면서 실행해보기
char result = '3';

void setup(){
    pinMode(VibrationPin, OUTPUT);
}

void loop(){
    if(delayOk){
        for(int i = 0; i < 2; i++){
            vibration(result);
        }
    }else{
        vibration(result);
    }
    delayOk = false;
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