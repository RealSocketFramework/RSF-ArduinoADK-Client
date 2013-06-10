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

RealSocket::RealSocket(const char *apiKey) : m_apiKey(apiKey){
  m_isConnected = false;
  int dataSize = 16;

  m_datas = (SyncData**)malloc(sizeof(m_datas) * dataSize);
  for(int i=0;i<dataSize;i++){
    m_datas[i] = NULL;
  }
}

void RealSocket::connect(){
  acc.powerOn();
}
void RealSocket::onConnected(void (*f)()){
  m_onConnected = f;
}
void RealSocket::onSyncUpdate(void (*f)(char id)){
  m_onSyncUpdate = f;
}
void RealSocket::update(){
  m_isConnectedAndroid = acc.isConnected();
  
  if (m_isConnectedAndroid) {
    while(true){
      int size[1] = {0};
      int length = acc.read(size,sizeof(size),1);
      if(length == -1){
        break;
      }
      Serial.print("read size:");
      Serial.println(size[0]);
      char* text = (char*)malloc(size[0]);
      if(text == NULL){
        Serial.print("malloc erro size:");
        Serial.println(size[0]);
        continue;
      }
      int textLength = acc.read(text,size[0],1);
      Serial.print("read text:");
      Serial.println(text);
      aJsonObject* json = aJson.parse(text);
      if(json != NULL){
        aJsonObject* typeObj = aJson.getObjectItem(json,"t");
        if(typeObj != NULL){
          String type = typeObj->valuestring;
          Serial.println("type:" + type +" is ws.connect:"+(type ==  "ws.connect"));
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
          }
        }
      
        free(text);
      }
      aJson.deleteItem(json);
    }
  }else{
    
  }
}
bool RealSocket::isConnected(){
  return m_isConnected;
}
void RealSocket::emit(aJsonObject *obj){
  char *text = aJson.print(obj);
  Serial.print("emit:");
  Serial.println(text);
  acc.write(text,strlen(text));
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
  data->m_type = SyncData::TYPE_INT;
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
     Serial.print("RealSocket::updateSyncByJson type is not supported!\n type:");
     Serial.println(type);
     return;
   }
   m_onSyncUpdate(id);
}

AndroidAccessory RealSocket::acc= AndroidAccessory(
   "MindFree, Inc.",            // Organization
   "RealSocketFramework",     // App name
   "Real Socket Framework v0.1.0",// Message
   "0.1.0",                       // Version
   "http://realsocketframework.com",        // url
   "0000000012345678"  // Serial number
);