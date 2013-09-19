/*
 * Copyright (c) 2013  Real Socket Framework com , http://realsocketframework.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "RealSocket.h"

//#define TRACE_RSF
//#define TRACE_RSF1

RealSocket::RealSocket(const char *apiKey) : m_apiKey(apiKey){
  m_isConnectedAndroid = false;
  m_isConnected = false;
  m_onMessageCallback = onMessageCallbackDefault;
  int dataSize = 16;

  m_datas = (SyncData**)malloc(sizeof(m_datas) * dataSize);
  for(int i=0;i<dataSize;i++){
    m_datas[i] = NULL;
  }
  
}

void RealSocket::connect(){
  if (usb.Init() == -1) {
    Serial.print("Usb.Init() failed to assert");
  }
}
void RealSocket::onMessageCallbackDefault(aJsonObject* obj){}
void RealSocket::onMessage(void (*f)(aJsonObject* obj)){
  m_onMessageCallback = f;
}
void RealSocket::onConnected(void (*f)()){
  m_onConnected = f;
}

void RealSocket::onError(void(*f)(int errorCode)){
	m_onError = f;
}
void RealSocket::onSyncUpdate(void (*f)(char id)){
  m_onSyncUpdate = f;
}
void RealSocket::update(){
  usb.Task();
  
  bool connectedDroid = isConnectedAndroid();
  bool connectedDroidStart = false;
  bool connectedDroidEnd = false;
  if(connectedDroid && !m_isConnectedAndroid){
    connectedDroidStart = true;
  }
  if(!connectedDroid && m_isConnectedAndroid){
    connectedDroidEnd;
  }
  m_isConnectedAndroid = connectedDroid;
  
  if(connectedDroidStart){
    
  }
  
  if (connectedDroid) {
    while(true){
      int32_t msgSizeBuffer[1] = {0};
      uint16_t len = sizeof(msgSizeBuffer);
      
      unsigned long prevTime1 = millis();
      uint8_t rcode  = acc.RcvData(&len, (uint8_t*)msgSizeBuffer);
      unsigned long time1 = millis();
#ifdef TRACE_RSF1
      if( 0 == len ) {
	      break;
      }
      Serial.print("header size:");
      Serial.println(len);
#endif
      int msgSize = msgSizeBuffer[0];
      if(msgSize == 0){
        break;
      }
#ifdef TRACE_RSF1
      Serial.println("---start---");
      Serial.print("msg size:");
      Serial.println(msgSize);
#endif
      uint8_t* textSource = (uint8_t*)malloc(msgSize*2);
      if(textSource == NULL){
        Serial.print("malloc error size:");
        Serial.println(msgSize);
        continue;
      }
      
      unsigned long prevTime = millis();
      uint16_t textLen = msgSize*2;//sizeof(textSource);
      for( uint16_t rlen = 0; rlen < msgSize; rlen +=  textLen ) {
          delay(2); 
		  textLen = msgSize*2 - rlen;
	      uint8_t textRcode = acc.RcvData(&textLen, textSource+rlen);
	      if(textRcode != 0 && textRcode != 4) {
		    Serial.print("RCODE:");Serial.println(textRcode);
    	  	break;
		  }
#ifdef TRACE_RSF1
      Serial.print("read size:");
      Serial.println(textLen);
		if( 0 == textLen ) {
	      Serial.println("### read size is zero ###");
      	}
#endif
	  }
      
      char* text = (char*)malloc(textLen + 1);
      memset(text,0,textLen + 1);
      memcpy(text,textSource,textLen);
      
#ifdef TRACE_RSF
	  unsigned long time = millis();
	  int period = time-prevTime;
	  if( period < 0 ) { period = 0; }  
	  Serial.print("read text[");
	  Serial.print(time1-prevTime1); Serial.print("+");
	  Serial.print(period); Serial.print("=");
	  Serial.print(time-prevTime1); 
	  Serial.print("]:");
	  Serial.println(text);
#endif
      aJsonObject* json = aJson.parse(text);
      if(json != NULL){
        aJsonObject* typeObj = aJson.getObjectItem(json,"t");
        if(typeObj != NULL){
          String type = typeObj->valuestring;
          
#ifdef TRACE_RSF1
          Serial.println("type:" + type +" is ws.connect:"+(type ==  "ws.connect"));
#endif
          if(type == "ws.connect"){
             emitStringValue("apiKey",m_apiKey);
          }else if(type == "init"){
            aJsonObject* syncDatas = aJson.getObjectItem(json,"syncDatas");
            char size = aJson.getArraySize(syncDatas);
            for(int i=0;i<size;i++){
              aJsonObject* data = aJson.getArrayItem(syncDatas,i);
              updateSyncByJson(data);
            }
            m_isConnected = true;
            m_onConnected();
          }else if(type == "sync"){
            aJsonObject* data = aJson.getObjectItem(json,"data");
            updateSyncByJson(data);
          }else if(type == "error"){
          	  int errorCode = aJson.getObjectItem(json,"errorCode")->valueint;
          	  aJsonObject* messageObj = aJson.getObjectItem(json,"message");
          	  if(messageObj != NULL){
          	  	  String message = messageObj->valuestring;
#ifdef TRACE_RSF
          	      Serial.print("error errorCode:");
          	      Serial.print(errorCode);
          	      Serial.println(" message:" + message);
#endif
          	  }
          	  m_onError(errorCode);
          }
          m_onMessageCallback(json);
        }
      }
      aJson.deleteItem(json);
      free(text);
      free(textSource);
#ifdef TRACE_RSF1
      Serial.println("---end---");
#endif
    }
  }else{
    
  }
}
bool RealSocket::isConnectedAndroid(){
  return acc.isReady();
}
bool RealSocket::isConnected(){
  return m_isConnected;
}
void RealSocket::emit(aJsonObject *obj){
  char *text = aJson.print(obj);
  uint8_t size = strlen(text);
  uint8_t* textBuffer = (uint8_t*)malloc(size);
  memset(textBuffer,0,size);
  memcpy(textBuffer,text,size);
  //TODO:
  unsigned long prevTime = millis();
  
  acc.SndData(size,textBuffer);
  
#ifdef TRACE_RSF
  unsigned long time = millis();
  int period = time-prevTime;
  if( period < 0 ) { period = 0; }  
  Serial.print("emit[");
  Serial.print(period);
  Serial.print("]:");
  Serial.println(text);
#endif
  
  free(textBuffer);
  free(text);
}

void RealSocket::emitStringValue(const char *type,const char *value){
   aJsonObject* obj = aJson.createObject();
   aJson.addStringToObject(obj,"t",type);
   aJson.addStringToObject(obj,"v",value);
   emit(obj);
   aJson.deleteItem(obj);
}
void RealSocket::emitIntValue(const char *type,int value){
   aJsonObject* obj = aJson.createObject();
   aJson.addStringToObject(obj,"t",type);
   aJson.addNumberToObject(obj,"v",value);
   emit(obj);
   aJson.deleteItem(obj);
}

void RealSocket::initSyncInt(char id,int value){
  aJsonObject* obj = aJson.createObject();
   aJson.addStringToObject(obj,"t","sync.init");
   aJson.addNumberToObject(obj,"v",value);
   aJson.addNumberToObject(obj,"id",id);
   emit(obj);
   aJson.deleteItem(obj);
}
void RealSocket::syncInt(char id,int value){
  SyncData* data = m_datas[id];
  if(data == NULL){
    initSyncInt(id,value);
    return;
  }

   aJsonObject* obj = aJson.createObject();
   aJson.addStringToObject(obj,"t","sync");
   aJson.addNumberToObject(obj,"v",value);
   aJson.addNumberToObject(obj,"version",data->version());
   aJson.addNumberToObject(obj,"id",id);
   emit(obj);
   aJson.deleteItem(obj);
}
int RealSocket::getSyncInt(char id){
  SyncData* data = m_datas[id];
  if(data == NULL){
    return 0;
  }
  return data->m_valueInt;
}
void RealSocket::updateSyncInt(char id,int value,int version){
  if(m_datas[id] == NULL){
    m_datas[id] = SyncData::newItem(id);
  }
  SyncData* data = m_datas[id];
  //data->m_type = SyncData::TYPE_INT;
  data->m_version = version;
  data->m_valueInt = value;
}
void RealSocket::updateSyncByJson(aJsonObject* data){
   char id =  aJson.getObjectItem(data,"id")->valueint;
   int version = aJson.getObjectItem(data,"version")->valueint;
   aJsonObject* valueObject = aJson.getObjectItem(data,"v");
   char type = valueObject->type;
   if(type == aJson_Int){
     int value = valueObject->valueint;
     updateSyncInt(id,value,version);
   }else{
#ifdef TRACE_RSF
     Serial.print("RealSocket::updateSyncByJson type is not supported!＼n type:");
     Serial.println(type);
#endif
     return;
   }
   m_onSyncUpdate(id);
}

USB RealSocket::usb;

ADK RealSocket::acc = ADK(&usb,
   "MindFree, Inc.",                   // The first argument: Organization name
   "RealSocketFramework",              // The second argument: Title name of the application not to exist
   "Real Socket Framework v0.2.0",     // The third argument: Dialogue indication message
   "0.2.0",                            // The fourth argument: Version
   "http://realsocketframework.com",   // The fifth argument: Jump URL
   "0000000012345678"                  // The sixth argument: Serial number
);
