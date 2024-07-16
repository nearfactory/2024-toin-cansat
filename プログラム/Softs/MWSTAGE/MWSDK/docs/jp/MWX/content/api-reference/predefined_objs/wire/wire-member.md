---
title: "Wire (メンバ関数版)"
author: "Mono Wireless Inc."
description: "Wire (メンバ関数版)"
---

# Wire (メンバ関数版)

メンバ関数を利用した方法は、抽象度が比較的低く、C言語ライブラリで提供されるような一般的なAPI体系に倣っています。二線シリアルバスの操作手続きがより直感的です。

ただしバスの利用の開始と終了を明示的に意識して記述する必要があります。

## 読み込み

### requestFrom()

```cpp
size_type requestFrom(
    uint8_t u8address,
    size_type length,
    bool b_send_stop = true)
```

指定バイト数分を一括で読み出します。読みだした結果はキューに保存されるため、直後にキューが空になるまで `.read()` メソッドを呼び出すようにしてください。

| パラメータ              | 解説                                |
| ------------------ | --------------------------------- |
| `u8address`        | 読み出し対象のI2Cアドレス                    |
| `length`           | 読み出しバイト数                          |
| `b_send_stop=true` | `true` の時、読み込み終了時に`STOP`ビットを設定する。 |
| 戻り値型 `size_type`   | 読み出したバイト数。 `0` は読み出しの失敗。          |

#### コード例

```cpp
int len = Wire.requestFrom(0x70, 6);
for (int i = 0; i < 6; i++) {
  if (Wire.available()) {
		  au8data[i] = Wire.read();
    Serial.print(buff[i], HEX);
  }
}
// skip the rest (just in case)
// while (Wire.available()) Wire.read(); // normally, not necessary.

```



## 書き出し

書き出し処理は、`beginTransmission()` を実行後、`write()` メソッドにより行います。一連の書き出しが終了したら `endTranmission()` を呼びます。

```cpp
	#define DEV_ADDR (0x70)
	const uint8_t msg[2] = 
	  {SHTC3_SOFT_RST_H, SHTC3_SOFT_RST_L};

	Wire.beginTransmission(DEV_ADDR);
	Wire.write(msg, sizeof(msg));
	Wire.endTransmission();
```

###

### beginTransmission()

```cpp
void beginTransmission(uint8_t address)
```

書き出しの転送を初期化する。書き出し処理終了後、速やかに `endTransmission()` を呼び出す。&#x20;

| パラメータ       | 解説             |
| ----------- | -------------- |
| `u8address` | 書き出し対象のI2Cアドレス |

###

### write(value)

```cpp
size_type write(const value_type value)
```

１バイトの書き出しを行う。

| パラメータ           | 解説                 |
| --------------- | ------------------ |
| `value`         | 書き込むバイト            |
| 戻り値 `size_type` | 書き込んだバイト数。`0`はエラー。 |



### write(\*value, quantity)

```cpp
size_type write(
  const value_type* value,
  size_type quantity)
```

バイト列の書き出しを行います。

| パラメータ           | 解説               |
| --------------- | ---------------- |
| `*value`        | 書き込むバイト列         |
| `size_type`     | バイト数             |
| 戻り値 `size_type` | 書き込んだバイト数。0はエラー。 |



### endTransmission()

```cpp
uint8_t endTransmission(bool sendStop = true)
```

書き出しの終了処理を行います。&#x20;

| パラメータ             | 解説             |
| ----------------- | -------------- |
| `sendStop = true` | STOPビットを発行します。 |
| 戻り値 `uint8_t`     | 0: 成功 4: 失敗    |

