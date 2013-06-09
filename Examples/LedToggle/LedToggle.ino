#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>
#include <aJSON.h>
#include <RealSocket.h>

//Set your api key and Create RealSocket object.
RealSocket rs("xxxx");

const int buttonPin = 7;
const int ledPin =10;
const int ID_COUNT = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin, INPUT);

  rs.onConnected(&onConnected);
  rs.onSyncUpdate(&onSyncUpdate);
  rs.connect();
}

void onConnected(){
  rs.initSyncInt(ID_COUNT,0);
}

void onSyncUpdate(char id){
  if(id == ID_COUNT){
    int value = rs.getSyncInt(id);
	if(value % 2 == 0){
		digitalWrite(greenPin,HIGH);
	}else{
		digitalWrite(greenPin,LOW);
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