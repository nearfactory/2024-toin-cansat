# ToCoNet_vDebugLevel()

## 解説

TWELITE NET のデバッグ出力レベルを変更する。

レベルは 0 が出力なしで、最大5です。大きくなればなるほど、多くのメッセージが出力されます。

{% hint style="warning" %}
[`ToCoNet_vDebugInit()`](toconet_vdebuginit.md) により初期化します。

[Makefile](../../twelite-sdk-howto/birudonitsuite/makefile-nitsuite.md) にて`TOCONET_DEBUG`を`1`に設定します。
{% endhint %}

{% hint style="danger" %}
デバッグメッセージは TWELITE NET の内部開発向けで、出力書式等については開示しません。問題の解析等を行う際に、お客様に出力をご連絡いただく場合があります。
{% endhint %}

## 引数

| 型       | 名前      | 詳細                                 |
| ------- | ------- | ---------------------------------- |
| `uint8` | `u8lvl` | 新しいデバッグ出力レベル。(0: 出力なし、1...5: 出力あり) |

## 戻り値

なし。

## サンプル

```c
    // get a serial input byte.
	case 'd': case 'D':
		_C {
			static uint8 u8DgbLvl;

			u8DgbLvl++;
			if(u8DgbLvl > 5) u8DgbLvl = 0;
			ToCoNet_vDebugLevel(u8DgbLvl);

			vfPrintf(&sSerStream, "set NwkCode debug level to %d.", u8DgbLvl);
		}
		break;
```

