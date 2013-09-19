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

#ifndef __RealSocket_h__
#define __RealSocket_h__

#include <Usb.h>
#include <adk.h>
#include <aJSON.h>
#include "SyncData.h"

class RealSocket{
  private:
    static void onMessageCallbackDefault(aJsonObject* obj);
    const char *m_apiKey;
    bool m_isConnectedAndroid;
    bool m_isConnected;
    void (*m_onMessageCallback)(aJsonObject* obj);
    void (*m_onConnected)();
    void (*m_onError)(int errorCode);
    void (*m_onSyncUpdate)(char id);
    SyncData **m_datas;
    void updateSyncInt(char id,int value,int version);
    void updateSyncByJson(aJsonObject* json);
  public:
    static ADK acc;
    static USB usb;
    RealSocket(const char *apiKey);
    void connect();
    void update();
    bool isConnectedAndroid();
    bool isConnectedWebSocket();
    bool isConnected();
    void emit(aJsonObject *obj);
    void emitStringValue(const char *type,const char *value);
    void emitIntValue(const char*type,int value);
    void onMessage(void (*f)(aJsonObject* obj));
    void onConnected(void (*f)());
    void onSyncUpdate(void (*f)(char id));
    void onError(void(*f)(int errorCode));
    void initSyncInt(char id,int value);
    void syncInt(char id,int value);
    int getSyncInt(char id); 
};

#endif /* __RealSocket_h__ */