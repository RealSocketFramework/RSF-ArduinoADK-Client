/**
 * Template code for the Real Socket Framework(RSF)
 */
#include <Usb.h>
#include <aJSON.h>
#include <RealSocket.h>

///////////////////////////////////////////////////
// Set your api key and Create RealSocket object.
RealSocket rs("Your API Key");
// TODO: Define synchronized data ID(0-15) between some devices with same api key.
const int ID_SYNC_DATA = 0;


void setup() {
  Serial.begin(9600);
  // TODO: add initialization code for hardware.
  // ...
  
  /////////////////////////////////////////////////
  // Initializing RealSocket object.
  rs.onConnected(&onConnected);    // Set connected callback function.
  rs.onSyncUpdate(&onSyncUpdate);  // Set updated callback function.
  rs.onError(&onError);            // Set error callback function.
  rs.connect();                    // Connect to the RSF server.
}

void loop() {
  rs.update();
  if(rs.isConnected()){
    // TODO: add application code using the RSF API.
    // ...
    
  }
  delay(17);
}

/**
 * Callback function when the RSF server is connected
 */
void onConnected() {
  Serial.println("RSF onConnected");
  // TODO: add initialization code for application.
  // ...
  // ex.) Initialize synchronized data.
  rs.initSyncInt(ID_SYNC_DATA, 0);
}

/**
 * Callback function when data synchronize
 * 
 * id : Updated data id
 */
void onSyncUpdate(char id) {
  Serial.println("RSF onSyncUpdate");
  // TODO: add application code.
  // ...
  // ex.) Read synchronized data.
  int value = rs.getSyncInt(id);
  Serial.println(value);
}

/**
 * Callback function when an error occurs
 * 
 * errorCode : happened error code
 */
void onError(int errorCode) {
  // TODO: add error handling.
  // ...
  Serial.print("RSF onError: error code = ");
  Serial.println(errorCode);
}

