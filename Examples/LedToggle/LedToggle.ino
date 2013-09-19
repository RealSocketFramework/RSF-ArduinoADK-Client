/**
 *  LedToggle : Sample code for the Real Socket Framework(RSF)
 */
#include <Usb.h>
#include <aJSON.h>
#include <RealSocket.h>

//Set your api key and Create RealSocket object.
RealSocket rs("Your API Key");
const int ID_COUNT = 0;

const int buttonPin = 7;
const int ledPin = 10;

int  prevButtonState = LOW;


void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin, INPUT);

  rs.onConnected(&onConnected);
  rs.onSyncUpdate(&onSyncUpdate);
  rs.connect();
}

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


void onConnected(){
  Serial.println("onConnected RealSocket");
  rs.initSyncInt(ID_COUNT,0);
}

void onSyncUpdate(char id){
  if(id == ID_COUNT){
    int value = rs.getSyncInt(id);
      if(value % 2 == 0){
        digitalWrite(ledPin,HIGH);
      }else{
        digitalWrite(ledPin,LOW);
      }
  }
}

void onError(int errorCode) {
  Serial.print("RSF onError: error code = ");
  Serial.println(errorCode);
}


