# アドレスの指定

## 単純ネットの場合

送り主と宛先アドレスの指定方法はロングアドレスとショートアドレスの２種類あります。

### ロングアドレス

TWELITE の32bitモジュールシリアル番号(32bit)を指定します。アドレスは ToCoNet_u32GetSerial() API により取得できます。

また、TWELITE モジュールのシールド缶に表示されている７桁の１６進数の S/N (シリアル番号) に 0x80000000 を加えたものになります。例えば 1012ABC というシリアル番号の場合は 0x81012ABC となります。

{% hint style="warning" %}
宛先をロングで指定する場合、パケットのペイロードサイズは小さくなります。（[パケットの最大長](pakettono.md)）
{% endhint %}

{% hint style="info" %}
IEEE802.15.4 規格では、64bit の MAC アドレスを使用しています。32bitモジュールアドレスを元に 64bit MAC アドレスを指定しています。
{% endhint %}

### ショートアドレス

0x0000 〜 0x0FFF までのアドレスを指定できます。アドレスは [cbAppColdStart](../../twelite-net-api-ref/krubakku/cbappcoldstart.md)() 関数中で　[sToCoNet_AppContext](../../twelite-net-api-ref/gou-zao-ti/stoconet_appcontext.md).u16ShortAddress に設定します。

### 同報通信（ショートアドレス 0xFFFF）

同報通信を行う場合に使用するアドレスです。



