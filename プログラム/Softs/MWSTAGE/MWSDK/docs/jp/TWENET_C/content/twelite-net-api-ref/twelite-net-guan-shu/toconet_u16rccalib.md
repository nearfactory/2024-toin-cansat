# ToCoNet_u16RcCalib()

## 解説 <a href="jie" id="jie"></a>

​ RCタイマーをキャリブレーションします。

* キャリブレーションの実行
* キャリブレーション値の取得
* キャリブレーション値の設定

{% hint style="info" %}
キャリブレーション値の取得には `ToCoNet_u16GetRcCalib()` も利用できます。
{% endhint %}

### 32768Hz 外部オシレータの利用

キャリブレーションは不要で 固定値 `9766` (32000/32768) カウントを指定します。[`cbAppColdStart()`](../krubakku/cbappcoldstart.md) にて以下の例のように指定します。

```c
// TWENET ライブラリ 1.2.0 以降のみで有効
void cbAppColdStart(bool_t bAfterAhiInit)
{
	if (!bAfterAhiInit) {
		ToCoNet_REG_MOD_ALL();
	} else {
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);
		...
		// システム起動時のキャリブレーションの省略
		sToCoNet_AppContext.bSkipBootCalib = TRUE;
		// 固定値の設定(v1.2以降)
		ToCoNet_u16RcCalib(9766);
	}
}

void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg) {
        
     if (eEvent == E_EVENT_START_UP) { //システム始動時
     	// 固定値の設定
 		ToCoNet_u16RcCalib(9766);
     }
 }

```

## 引数 <a href="yin-shu" id="yin-shu"></a>

| 型         | 名前        | 値            | 内容                                             |
| --------- | --------- | ------------ | ---------------------------------------------- |
| ​`uint16` | `​u16val` | `0`          | キャリブレーションを実施する。キャリブレーションには1ms未満ですが、若干の時間を要します。 |
|           |           | `5000-15000` | キャリブレーション値を設定します。                              |
|           |           | `0xFFFF`     | 設定値を取得します。                                     |

## 戻り値 <a href="ri" id="ri"></a>

| 型         | 詳細                                           |
| --------- | -------------------------------------------- |
| `​uint16` | `u16val == 0xFFFF` のときに、現在の設定値を返す。それ以外は未定義。​ |

## サンプル <a href="sanpuru" id="sanpuru"></a>

```c
vfPrintf(&sSerStream, "RC Calib = %d"LB,  ToCoNet_u16GetRcCalib());
```

