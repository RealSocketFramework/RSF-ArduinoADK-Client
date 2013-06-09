RSF-ArduinoADK-Client v0.1.0
=====================
http://realsocketframework.com/
  
RealSocketFrameworkはArduino同士をWebSocket通信でデータを同期できるフレームワークです。
WebSocketでの通信はRealSocketFrameworkが提供しているサーバを経由して自動的に通信することになるので、
サーバサイドのプログラミングは一切不要で、Arudinoスケッチのみの開発だけで通信することができます。


RSF-ArduinoADK-Client v0.1.0はArduino ADKボードとAndroidをUSBケーブルで接続して、
RealSocketFrameworkサーバと通信するためのライブラリです。
サーバとのWebSocket通信はAndroidから行い、
ArduinoとAndroid間の通信はUSBケーブル経由でADKを使用して通信しています。

RealSocketFrameworkは将来的には、Bluetooth経由やイーサネットシールドからでも
通信できるようにFrameworkを拡張していく予定です。

* [QuickStart](https://github.com/RealSocketFramework/RSF-ArduinoADK-Client/wiki/QuickStart)
* [Document](https://github.com/RealSocketFramework/RSF-ArduinoADK-Client/wiki/Document)

##License
[RealSocket](https://github.com/RealSocketFramework/RSF-ArduinoADK-Client/tree/master/Arduino-Libraries/RealSocket)
のライセンスはMIT License。

##ライブラリ
RealSocketFrameworkでは以下のライブラリを使用しています。
* [aJson](https://github.com/interactive-matter/aJson)
* [USB_Host_Shield](https://github.com/felis/USB_Host_Shield)
* [AndroidAccessory](http://developer.android.com/tools/adk/adk2.html)
