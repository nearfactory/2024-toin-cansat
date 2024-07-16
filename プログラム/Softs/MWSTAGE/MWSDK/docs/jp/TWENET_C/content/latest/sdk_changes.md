# TWELITE SDK 改版履歴

MWSDK2020\_05以降の履歴情報は以下に記載します。

{% embed url="https://github.com/monowireless/MWSDK_COMMON/blob/master/ReleaseNotes%2Emd" %}



## MWSDK2020\_05

{% hint style="danger" %}
2019/12, 2020/04リリースのMWSDKを利用の場合は、更新を推奨します。
{% endhint %}

### 更新内容

* \[TWENET] イベントキューがスタックする場合があった
* \[TWENET] スリープ復帰後にシリアルポート出力がしない場合があった
* \[TWENET] スリープ復帰後に無線パケットの送受信キューのクリアをデフォルトの振る舞いとした
* \[TWENET] 重複チェッカ DUPCHK モジュールのパラメータを変更できるようにした
* \[MWX] 重複チェッカ duplicate_checker の初期化等に不備があり期待通りの除去を行っていなかった
* \[MWX] format() の実装を機種依存の少ないものとした。また、引数を最大８までとした。64bit引数が含まれる場合は引数の数は制限される。
* MWSDK 以下 (Toolsフォルダ除く) を GitHub のレポジトリに格納した。

### 000manifest

```
NAME: MWSDK2020_05
DATE: 2020-05-29

COMMON FILES: https://github.com/monowireless/MWSDK_COMMON/releases/tag/MWSDK2020_05
  CHIP LIBRARY VERSION: SW4063V1416
  TWENET (TWELITE NET) C LIBRARY VERSION: L1303
  MWX C++ LIBRARY VERSION: https://github.com/monowireless/mwx/releases/tag/0.1.3
  TWESETTINGS LIBRARY: https://github.com/monowireless/twesettings/releases/tag/0.2.3
```



## MWSDK2020\_04

TWELITE STAGE に対応にした環境に変更しました。

* より記述内容が少なくなる Makefile に変更しました。\
  ※ 従来のプロジェクトでの Makefile から書き換えが必要になります。
* [mwx ライブラリ](https://mwx.twelite.info)に対応しました。



### TWELITE NET 1.2.0->1.3.0 の変更

* `Utils/Source/serial.[ch], uart.[ch]`UARTのflush時に異常なふるまい（文字化けなど）が発生する場合があった問題に対処。
* TickTimer, ADC完了イベントの消滅を抑制する実装をした。他のハードイベントが重なったときに内部キューに貯められず消滅する場合があるため、これら割り込み発生時には内部変数をを設定し優先してイベントコールバックを呼び出すようにした。cbAppColdStart(FALSE) 時に以下の変数を0に設定すれば無効化できる。 `volatile uint32 _u32_INT_ticktimer; volatile uint32 _u32_INT_adc_comp;`
* パケット送信要求のキューがフルかどうか判定して、フルの場合はエラーを返すようにした。
* WarmStart時に乱数生成が再開しなかった問題を修正。
* `tsEvent`構造体に`pvExtraData`を追加 (mwxライブラリでPRSEV状態遷移マシンを動作させるための修正)
* `volatile uint32 u32TickCount_ms`に変更(volatile の追加)
* メインループで CPU_Doze をコントロールするための `bool_t _b_cpu_doze_dirty`変数を追加。`TRUE`にセットしておくと、メインループのDOZEをスキップする。
* mwx ライブラリ利用のため、一部関数ヘッダ定義を外部参照可能に修正 (ユーザー非開示：PRSEV:イベント関連、内部構造体など)
* mwx ライブラリ利用のため、ヘッダファイルのメンテナンス (extern "C")
* `Utils/Source/utils.[ch]`: PWM設定で DUTY の刻みを細かく設定できるように修正。構造体メンバー u16duty_max に値を設定することで、刻み値を 1024 以外にも設定可能。刻みが1024,4096,16384の場合に除算実行の替わりにビットシフトにするようにした。
* `sToCoNet_AppContext.u32ChMask` の MSB を内部利用するように関連コードを一部修正。
* 内部チャネル変更関数中で、ソースコード上、現在の設定チャネルに変更される可能性があったコードを修正。



## TWELITE NET 1.2.0 (2018-05-14)

* TWELITE BLUE 用のSDKのツールチェインから、TWELITE RED 共用のツールチェインに変更
  * Windows用では cygwin から msys になります。
* 推奨開発環境を eclipse から VSCode に変更
* ライブラリ一式を 1.2.0 へ変更 (v1.0.6より)
  * TWELITE RED に対応
  * make のパラメータの整理 (make TWLIET={BLUE|RED})
  * 中継における上位ノードを決定する方法を、アクティブ方式の探索から、定期パケット（NBビーコン）を用いるパッシブ方式に変更 (App_Tagの中継器に実装)
  * 32Kオシレータを利用する sToCoNet_AppContext.u8Osc32Kmode を追加
  * アプリケーションID外のネットワークへの送信機能（実験的実装）
  * その他軽微な修正
    * 重複チェッカ(ToCoNet_USE_MOD_DUPCHK)のタイムスタンプ更新方法の改良
    * 稀にチャネルが正しく切り替わらなかった問題の修正

