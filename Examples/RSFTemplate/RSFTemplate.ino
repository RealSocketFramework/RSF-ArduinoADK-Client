#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>
#include <aJSON.h>
#include <RealSocket.h>

//Set your api key and Create RealSocket object.
RealSocket rs("xxxx");
const int ID_COUNT = 0;

void setup() {
  Serial.begin(9600);
  rs.onConnected(&onConnected);
  rs.onSyncUpdate(&onSyncUpdate);
  rs.connect();
}

void onConnected(){
  rs.initSyncInt(ID_COUNT,0);
  Serial.println("RSF onConnected");
}

void onSyncUpdate(char id){
  int value = rs.getSyncInt(id);
  Serial.println("RSF onSyncUpdate");
  Serial.println(value);
}

void loop(){
  rs.update();
  if(rs.isConnected()){
  }
  delay(17);
}