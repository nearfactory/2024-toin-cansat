---
title: "SPI (メンバ関数版)"
author: "Mono Wireless Inc."
description: "SPI (メンバ関数版)"
---
# SPI (メンバ関数版)

`begin()`メソッドによりハードウェアの初期化を行った後、`beginTransaction()`によりバスの読み書きができるようになります。`beginTransaction()`を実行するとSPIのセレクトピンが選択されます。読み書きは`transfer()`関数を用います。SPIは読み出しと書き込みを同時に実行します。



### beginTransaction()

```cpp
void beginTransaction()
void beginTransaction(SPISettings settings)
```

バスの利用開始を行います。SPIのセレクトピンをセットします。

`settings`パラメータを与えて呼び出した場合は、バスの設定を行います。



### endTransaction()

```cpp
void endTransaction()
```

バスの利用を終了します。SPIのセレクトピンを解除します。



### transfer(), transfer16(), transfer32()

```cpp
inline uint8_t transfer(uint8_t data)
inline uint16_t transfer16(uint16_t data)
inline uint32_t transfer32(uint32_t data)
```

&#x20;バスの読み書きを行います。`trasnfer()`は8bit、`transfer16()`は16bit、`transfer32()`は32bitの転送を行います。
