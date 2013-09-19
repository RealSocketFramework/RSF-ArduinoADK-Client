
/**
 *  LedSwiring : Sample code for the Real Socket Framework(RSF)
 */
#include <Usb.h>
#include <aJSON.h>
#include <RealSocket.h>

RealSocket rs("Your API Key");
const int ID_COUNT = 1;

const int buttonPin= 7;
const int greenPin = 10;
const int bluePin  = 11;
const int redPin   = 12;

int  prevButtonState = LOW;


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
  
  // Initializing RealSocket object.
  rs.onConnected(&onConnected);    // Set connected callback function.
  rs.onSyncUpdate(&onSyncUpdate);  // Set updated callback function.
  rs.onError(&onError);            // Set error callback function.
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

void onError(int errorCode) {
  Serial.print("RSF onError: error code = ");
  Serial.println(errorCode);
}

