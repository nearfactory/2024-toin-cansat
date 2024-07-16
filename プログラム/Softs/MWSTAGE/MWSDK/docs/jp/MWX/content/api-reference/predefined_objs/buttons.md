---
title: "Buttons"
author: "Mono Wireless Inc."
description: "ディジタル入力管理クラス (mwx::periph_buttons)"
---

# Buttons

ディジタル入力の変化を検出します。このクラスは、同じ検出値が複数回得られたときに変化を検出します。メカ式のボタンのチャタリングの影響を小さくするのに有効です。



## メソッド

### setup()

```cpp
void setup(uint8_t max_history);
```

パラメータの`max_history`は、`begin()`で設定可能な参照回数の最大値です。ここではメモリーの確保と初期化を行います。



### begin()

```cpp
void begin(uint32_t bmPortMask,
				   uint8_t u8HistoryCount,
				   uint16_t tick_delta);
```

`Buttons`の動作を開始します。１番目のパラメータ`bmPortMask`は監視対象のディジタル入力のビットマップを指定します。bit 0がDIO 0, ... , bit N がDIO Nに対応します。複数指定することができます。２番目の`u8HistoryCount`は値の確定をするのに必要な回数です。３番目の`tick_delta`は値の確認を行う間隔をmsで指定します。

値の確定には`u8HistoryCount*tick_delta`\[ms]かかることになります。例えば`u8HistoryCount`=5, `tick_delta`=4の場合は、状態の確定に最低約20msかかります。

確認は`TickTimer`のイベントハンドラで行っています。割り込みハンドラではないので、処理等の遅延の影響を受けますが、メカ式ボタン等のチャタリング抑制には十分です。



### end()

```cpp
void end()
```

`Buttons`の動作を終了します。



### available()

```cpp
inline bool available()
```

変化が検出されたときに`true`を返します。`read()`を実行するとクリアされます。



### read()

```cpp
bool read(uint32_t& u32port, uint32_t& u32changed)
```

availableになったとき呼び出します。`u32port`は現在の入力DIOのビットマップ、`u32changed`は変化が検出されたDIOのビットマップです。

Buttonsが動作していない場合は`false`を返します。



## 動作について

### 初回の値確定

Buttonsが動作を開始した時点では、DIOの入力状態は未確定です。値が確定した時点でavailableになります。このとき`read()`で読み出すビットマップのMSB(bit31)が１にセットされます。

動作確定を要するため、入力値が定常的に変化するポートを監視する目的では利用できません。



### スリープ

スリープ前にButtonsが稼働状態であれば、復帰後に再開します。再開後、初回確定を行います。





