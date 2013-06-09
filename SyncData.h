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

#ifndef __SyncData_h__
#define __SyncData_h__

class SyncData{
  friend class RealSocket;
  public:
    static const char TYPE_INT = 0;
    static const char TYPE_FLOAT = 1;
    static const char TYPE_STRING = 2;
    int id();
    char type();
    int version();
  private:
    static SyncData* newItem(char id);
    static void deleteItem(SyncData* data);
    char m_id;
    int m_version;
    char m_type;
    union{
      int m_valueInt;
      double m_valueFloat;
      char *m_valueString;
    };
};

#endif /* __SyncData_h__ */