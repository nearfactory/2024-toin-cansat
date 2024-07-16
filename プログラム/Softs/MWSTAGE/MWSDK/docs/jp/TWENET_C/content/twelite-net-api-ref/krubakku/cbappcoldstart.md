# cbAppColdStart()

## 解説

`cbAppColdStart(bool_t bStart)` は、`bStart == FALSE` で最初に呼び出され、その後 `bStart == TRUE` で呼び出されます。`bStart == TRUE` の節でアプリケーションの初期化処理を行います。

ここで TWELITE NET の重要な初期化 ([`sToCoNet_AppContext`](../gou-zao-ti/stoconet_appcontext.md) 構造体) を行う必要が有ります。一部の機能はここでしか設定できません。

[`ToCoNet_Event_Register_State_Machine()`](../yzaibento/toconet_event-api/toconet_event_register_state_machine.md) 関数にて、ユーザ定義イベント処理関数を登録しています。

この処理の最後で [`ToCoNet_vMacStart()`](../twelite-net-guan-shu/toconet_vmacstart.md) 関数を呼び出します。

API の手続きとして MAC 層の開始を明示しています。

## 引数

| 型        | 名前       | 詳細                                    |
| -------- | -------- | ------------------------------------- |
| `bool_t` | `bStart` | 起動時に２回呼び出され、最初が`FALSE`、2回目が`TRUE`となる。 |

#### bStart

`FALSE`：マイコンの起動直後で、ペリフェラル等の初期化も行われていない。ここでモジュール登録の [`ToCoNet_REG_MOD_ALL()`](../twelite-net-makuro/toconet_reg_mod_all.md)マクロを呼び出す。

`TRUE`：TWELITE NET ライブラリやハードウェアAPIの初期化が行わた後、ライブラリの初期化前で呼び出される。ここでは以下の手続きを記述する。

* [`sToCoNet_AppContext`](../gou-zao-ti/stoconet_appcontext.md) 構造体に初期パラメータの設定
* [`ToCoNet_Event_Register_State_Machine()`](../yzaibento/toconet_event-api/toconet_event_register_state_machine.md) によりステートマシンの登録
* アプリケーション独自のメモリ領域等の初期化
* ハードウェアの初期化
* [`ToCoNet_vMacStart()`](../twelite-net-guan-shu/toconet_vmacstart.md) によるMAC 層の開始

## 戻り値

なし

## サンプル

```c
void cbAppColdStart(bool_t bAfterAhiInit)
{
	if (!bAfterAhiInit) {
		// before AHI init, very first of code.

		// Register modules
		ToCoNet_REG_MOD_ALL();

	} else {
		// disable brown out detect
		vAHI_BrownOutConfigure(0,//0:2.0V 1:2.3V
				FALSE,
				FALSE,
				FALSE,
				FALSE);

		// clear application context
		memset (&sAppData, 0x00, sizeof(sAppData));
		sAppData.u8channel = CHANNEL;

		// ToCoNet configuration
		sToCoNet_AppContext.u32AppId = APP_ID;
		sToCoNet_AppContext.u8Channel = CHANNEL;

		sToCoNet_AppContext.bRxOnIdle = TRUE;

		// others
		SPRINTF_vInit128();

		// Register
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);

		// Others
		vInitHardware(FALSE);

		// MAC start
		ToCoNet_vMacStart();
	}
}
```

