# adc.c

adc.c は、多くのサンプルコードの common/Source/adc.c として含まれています。

以下のコードが関係します。

| ファイル名           | 内容                  |
| --------------- | ------------------- |
| sensor_driver.h | センサー処理を抽象化（ヘッダファイル） |
| sensor_driver.c | 状態遷移を用いたセンサー処       |
| adc.h           | ADC処理部（ヘッダファイル）     |
| adc.c           | ADC処理部              |

{% hint style="warning" %}
サンプルごとにコードに違いがある場合があります。
{% endhint %}

### 利用方法

以下のコード中のコメントを参照してください。

```c
#include "sensor_driver.h"
#include "adc.h"

tsObjData_ADC sObjADC; // ADC管理構造体（データ部）
tsSnsObj sADC; // ADC管理構造体（制御部）
int16 a1,a2,ab; // 結果格納用

...
	// ADC 初期化
	vSnsObj_Init(&sADC);
	vADC_Init(&sObjADC, &sADC, TRUE);
	vADC_WaitInit(); // ハード初期化待ちを行う

...
	// ADC計測したいポートの指定（以下では電源電圧とADC1,2）
	sObjADC.u8SourceMask = TEH_ADC_SRC_VOLT
    	| TEH_ADC_SRC_ADC_1 | TEH_ADC_SRC_ADC_2;
    
	// ADC開始
	vSnsObj_Process(&sADC, E_ORDER_KICK); // 開始の号令

// ADCの１チャネル処理が終わるのを待って(=E_AHI_DEVICE_ANALOGUE割り込み)
// vSnsObj_Process() を順次呼び出す。
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	switch (u32DeviceId) {
	case E_AHI_DEVICE_ANALOGUE:
		// ADC完了割り込み
		vSnsObj_Process(&sADC, E_ORDER_KICK);
		if (bSnsObj_isComplete(&sADC)) {
			// 全チャネルの処理が終わった。
			// 値は以下に格納される
			a1=sObjADC.ai16Result[TEH_ADC_IDX_ADC_1]; // ADC1[mV]
			a2=sObjADC.ai16Result[TEH_ADC_IDX_ADC_2]; // ADC2[mV]
			ab=sObjADC.ai16Result[TEH_ADC_IDX_VOLT]; // 電源電圧[mV]
			
			// ADC開始前の初期状態に戻す
			vSnsObj_Process(&sADC, E_ORDER_KICK);
			
			// 連続して実行する場合はもう一度E_ORDER_KICK
			vSnsObj_Process(&sADC, E_ORDER_KICK);
		}
		break;

	default:
		break;
	}
}
```

### 関数

#### void vSnsObj_Init(tsSnsObj _\*_pSnsObj)

センサー管理構造体を初期化します。vADC_Init()の直前に呼び出します。

#### void vADC_Init(tsObjData_ADC \*pData,_ _tsSnsObj _\*_pSnsObj, bool_t bInitAPR)

ADCの初期化を行います。tsObjData構造体(結果格納用)とtsSnsObj構造体(ADC管理)はあらかじめ用意しておきます。

bInitAPRはTRUEの場合、ADCハードウェアの初期化を行います。ハードウェアの初期化には若干の時間が必要ですので、必ずvADC_WaitInit()を実行して初期化を待ちます。

#### void vSnsObj_Process(tsSnsObj \*pObj, teEvent eEv)

ADC処理の進捗させる。具体的には、ADCの１ポート分の変換完了するたびに本処理を呼び出す。ADCの変換完了を待つには、E_AHI_DEVICE_ANALOGUE割り込みを用いる。単純に一定時間(概ね100usec)経過した後でも構わない。

本処理中で、ADC値の取得と mV値への演算処理が行われ、tsSnsObj構造体中に値が格納される。

本処理は、tsSnsObj構造体で管理される状態遷移に対するイベント処理になる。本処理直後にbSnsObj_isComplete()を呼び出し処理が完了したかを確認する。再び初期状態に戻すには、改めてE_ORDER_KICKを引数として本処理を実行する（つまりADCを再度実行するには完了後に E_ORDER_KICK を２回実行することになる）。

#### tsObjData_ADC構造体

| メンバー             |                                                                                                                        |
| ---------------- | ---------------------------------------------------------------------------------------------------------------------- |
| u8SourceMask     | ADC対象のポートを指定するビットマップ。指定したポートがADC対象となる。- TEH_ADC_SRC_VOLT : 電源電圧- TEH_ADC_SRC_ADC_1-4 : ADC1,2,3,4 |
| u8InputRangeMask | ADC対象のポートのレンジ(0-Vrefまたは0-2Vref)を指定する。指定したポートは0-Vrefとなる。未指定の場合 0-2Vref となる。                                             |
| ai16Result\[]    | ADC値を保存する構造体。結果はmV値として格納される。- TEH_ADC_IDX_VOLT : 電源電圧- TEH_ADC_IDX_ADC_1-4: ADC1,2,3,4            |

####

