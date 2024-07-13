# SERIAL_vFlush()

## 解説

SERIAL ライブラリによる出力要求が終了するまで待つ。

{% hint style="info" %}
スリープ前に全てのメッセージを出力するような場合に利用します。通常は十分な出力用のFIFOバッファーを確保し、極力呼び出しさないようにする処理を推奨します。
{% endhint %}

{% hint style="warning" %}
出力完了待ちの処理で動作が不安定になる場合があります。この場合は、この待ち処理を削除して動作確認します。

待ち処理を省略し出力が途切れる場合は、出力用のFIFOメモリを余分に確保します。
{% endhint %}

## 引数 <a href="yin-shu" id="yin-shu"></a>

| 型     | 名前           | 詳細                                              |
| ----- | ------------ | ----------------------------------------------- |
| uint8 | u8SerialPort | シリアルポート (E_AHI_UART\_0 または E_AHI_UART\_1 を指定する） |

## 戻り値

なし。

## サンプル

```c
void cbToCoNet_vMain(void) {
	while (!SERIAL_bRxQueueEmpty(sSerPort.u8SerialPort)) {
		int16 i16Char;
​		i16Char = SERIAL_i16RxChar(sSerPort.u8SerialPort);
​		vfPrintf(&sSerStream, "\n\r# [%c] --> ", i16Char);
	    SERIAL_vFlush(sSerStream.u8Device);
		...
	}
}
```

