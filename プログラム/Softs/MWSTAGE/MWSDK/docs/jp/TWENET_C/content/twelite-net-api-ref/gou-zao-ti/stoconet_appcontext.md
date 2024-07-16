# sToCoNet_AppContext

TWELITE NET の動作を決める構造体で `sToCoNet_AppConetext` として静的に確保されています。始動時に呼び出されるコールバック関数 [`cbAppColdStart()`](../krubakku/cbappcoldstart.md)にて設定したり、始動後 `sToCoNetAppContext` を直接書き換え  [`ToCoNet_vRfConfig()`](../twelite-net-guan-shu/toconet_vrfconfig.md)  を呼び出すことで設定変更します。

| 定義名解説                                                 | 解説                                                                                                                                                                             |
| ----------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `uint32 u32AppId`<br>(必須、始動後変更不可)   | 32bitのアプリケーションID。本IDでTWELITE NET同士の識別を行います。値域： `????0000`, `????FFFF`, `0000????`, `FFFF????` は設定できません。規定値： `0xFFFFFFFF`。必ずアプリケーションから設定します。                                    |
| `uint32 u32ChMask`<br>(モジュールによって必須) | 利用するチャネル群。ch13 をマスクに加える場合は 1UL << 13 のビットを１にします。規定値: 0x07fff800UL (ch11～26)必須モジュール: CHANNEL_MGR, NBSCAN, NBSCAN_SLAVE, LAYERTREE                                               |
| `uint16 u16ShortAddress`                              | モジュールのショートアドレス。指定しなければモジュールのシリアル番号から自動生成されます。0xFFFFは指定出来ません。ネットワーク層利用時は指定できません。 規定値: モジュールシリアル番号から自動設定                                                                         |
| `uint8 u8Channel` (必須)                                | モジュールの利用チャネル。上記 `u32ChMask` に含まれるチャネルを指定します。※ [モジュール](../../twelite-net-api-expl/mojru.md)ToCoNet_USE_MOD_CHANNEL_MGR 利用時は設定不要ですが、アプリケーションからのチャネル変更は出来ません。値域: 11～26規定値: 18 |
| `uint8 u8CPUClk` (変更推奨せず)                             | 通常稼働時のCPUクロック。3:32MHz, 2:16Mhz, 1:8Mhz, 0:4Mhz を指定します。規定値: 2:16MHz                                                                                                             |
| `unit8 u8TxPower`                                     | モジュールの出力 3:最大 2: -11.5db 1: -23db 0:-34.5db となります。(規定値: 3 )                                                                                                                    |
| `uint8 u8TxMacRetry`                                  | MAC層の再送回数 0..7 を指定します。(規定値: 3)                                                                                                                                                 |
| `bool_t bRxOnIdle`                                    | TRUE:無線回路アイドル時も受信回路を動作させます。受信が必要な場合は必ずTRUEに設定にしますが、受信時は常に受信電流を消費します。規定値: FALSE (ネットワーク層では TRUE 必須)                                                                             |
| `uint8 u8CCA_Retry`                                   | CCA のリトライ回数  (通常は変更しません)                                                                                                                                                       |
| `uint8 u8CCA_Level `                                  | CCA アルゴリズムの開始レベル (通常は変更しません)                                                                                                                                                   |
| `uint8 u8Osc32Kmode`                                  | 32K 水晶のモード (0x00: 内蔵RC, 0x02: 32K水晶振動子, 0x03: 32K発振器)。外部オシレータを利用する際は、始動時にクロック安定化をするための時間が必要となります。                                                                              |
| `uint8 u8RandMode`                                    | 乱数生成方法の指定。0:内部ハードを利用(32K外部オシレータが利用されたときは XorShift を採用します)  3:XorShift法によるアルゴリズム(乱数の種は始動時に内部ハードより生成します)                                                                         |
| `uint16 u16TickHz` (変更推奨せず)                           | システムの Tick カウントの周期(1000で割り切れる値にすること。1000, 500, 250, 200, 100)規定値: 250 (4ms)                                                                                                    |
| `bool_t bSkipBootCalib`                               | 起床時の RC タイマーのキャリブレーション値の計算を省略する。明示的にキャリブレーションを設定するには [`ToCoNet_u16RcCalib()`](../twelite-net-guan-shu/toconet_u16rccalib.md) API を利用します。                                       |
| `uint8 u8MacInitPending`                              | TRUE ならMAC層の初期化の遅延実施する。センサー処理等のみで起床する場合など、時間と電流消費のある MAC 層の初期化をさせない目的で設定する。                                                                                                    |

{% hint style="warning" %}
ch26 は海外認証(FCC/IC)で使用できない場合があります(u8Channel, u32ChMask)
{% endhint %}
