RSF-ArduinoADK-Client v0.1.0
=====================
###限定トライアルユーザ受付中です(6/12～6/30)。2013年7月にβリリースを予定しています。


http://realsocketframework.com/
  
Real Socket FrameworkはモノとモノをRealtime Webで繋ぐ開発プラットフォームです。
WebSocketでの通信はReal Socket Frameworkが提供しているサーバを経由して自動的に通信します。
 サーバサイドのプログラミングは一切不要で、クライアントサイドのみの開発だけで通信することができます。


RSF-ArduinoADK-Client v0.1.0は[Arduino ADKボード](http://arduino.cc/en/Main/ArduinoBoardADK)とAndroidをUSBケーブルで接続して、
Real Socket Frameworkサーバと通信するためのライブラリです。
サーバとのWebSocket通信はAndroidから行い、
ArduinoとAndroid間の通信はUSBケーブル経由でADKを使用して通信しています。

Real Socket Frameworkは将来的には、Bluetooth経由や[イーサネットシールド](http://arduino.cc/en/Main/ArduinoEthernetShield)からでも
通信できるようにFrameworkを拡張していく予定です。

* [QuickStart](https://github.com/RealSocketFramework/RSF-ArduinoADK-Client/wiki/QuickStart)
* [Document](https://github.com/RealSocketFramework/RSF-ArduinoADK-Client/wiki/Document)

##License
[RealSocket](https://github.com/RealSocketFramework/RSF-ArduinoADK-Client/tree/master/Arduino-Libraries/RealSocket)
のライセンスはMIT Licenseです。

##ライブラリ
Real Socket Frameworkでは以下のライブラリを使用しています。
* [aJson](https://github.com/interactive-matter/aJson)
* [USB_Host_Shield](https://github.com/felis/USB_Host_Shield)
* [AndroidAccessory](http://developer.android.com/tools/adk/adk2.html)
