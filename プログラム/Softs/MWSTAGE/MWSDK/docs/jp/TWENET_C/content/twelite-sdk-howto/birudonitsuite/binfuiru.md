# binファイル命名則

binファイル名の基本の形は以下になります。

> {プロジェクト名}\_{バリアント名}\_{ARCH名}\_{TWENETライブラリバージョン}\_{Version.mkバージョン}
>
> 例: Samp_PingPong_PingPong_BLUE_L1200\_V0-1-4.bin

| 名前               | 解説                                                       | 例             |
| ---------------- | -------------------------------------------------------- | ------------- |
| プロジェクト名          | 通常プロジェクトの最上位フォルダ名です。                                     | Samp_PingPong |
| バリアント（派生）名       | 通常プロジェクト中のサブフォルダ名です。１プロジェクト中に親機用・子機用を別々に生成するよう場合に使います。   | PingPong      |
| ARCH名            | BLUE: TWELITE BLUE, RED: TWELITE RED                     | BLUE          |
| TWENETライブラリバージョン | バージョン識別子です。                                              | L1200         |
| Version.mk バージョン | [Version.mk ](versionmk-nitsuite.md)ファイルに記述されたバージョン番号です。 | V0-1-4        |

{% hint style="info" %}
ファイル名は [Makefile](makefile-nitsuite.md) により一部カスタマイズ可能です。
{% endhint %}

