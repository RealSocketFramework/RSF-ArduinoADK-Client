
#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>
#include <aJSON.h>
#include "RealSocket.h"

RealSocket rs("xxxx");

const int buttonPin = 7;
const int greenPin =10;
const int bluePin = 11;
const int redPin = 12;

void setup() {
  Serial.begin(9600);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(redPin,OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.println("setup");
  delay(200);
  digitalWrite(greenPin,HIGH);
  delay(200);
  digitalWrite(greenPin,LOW);
  digitalWrite(bluePin,HIGH);
  delay(200);
  digitalWrite(bluePin,LOW);
  digitalWrite(redPin,HIGH);
  delay(200);
  digitalWrite(redPin,LOW);
  delay(200);
  
  rs.onConnected(&onConnected);
  rs.onSyncUpdate(&onSyncUpdate);
  rs.connect();
}
const int ID_COUNT = 0;
void onConnected(){
  Serial.println('onConnected realSocket');
  rs.initSyncInt(ID_COUNT,0);
}
void onSyncUpdate(char id){
  if(id == ID_COUNT){
    digitalWrite(redPin,LOW);
    digitalWrite(bluePin,LOW);
    digitalWrite(greenPin,LOW);
  
    int value = rs.getSyncInt(id);
    if(value & 1){
      digitalWrite(greenPin,HIGH);
    }
    if(value & 2){
      digitalWrite(bluePin,HIGH);
    }
    if(value & 4){
      digitalWrite(redPin,HIGH);
    }
  }
}

int  prevButtonState = LOW;

void loop(){
  rs.update();
 if(rs.isConnected()){
   int buttonState = digitalRead(buttonPin);
   if(buttonState ==HIGH && prevButtonState == LOW){
     rs.syncInt(ID_COUNT,rs.getSyncInt(ID_COUNT) + 1 );
   }
   prevButtonState = buttonState;
 }
 delay(17);
}
