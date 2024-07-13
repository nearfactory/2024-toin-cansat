---
title: "Timer0 .. 4"
author: "Mono Wireless Inc."
description: "タイマー, PWM (mwx::periph_timer)"
---

# Timer0 .. 4

タイマーでは、指定周期でのソフトウェア割り込みを発生させる目的、指定周期でPWM出力を行う２つの機能があります。TWELITE無線モジュールには0..4まで合計５つのタイマーが利用可能です。

組み込みオブジェクト名は `Timer0..4` ですが、このページでは `TimerX`と記載します。

## メソッド

### setup()

```cpp
void setup()
```

タイマーを初期化します。この呼び出しにより必要なメモリ領域の確保を行います。



### begin()

```cpp
void begin(uint16_t u16Hz, bool b_sw_int = true, bool b_pwm_out = false)
```

タイマーを開始します。１番目のパラメータは、タイマーの周期でHzで指定します。２番目のパラメータを`true`にするとソフトウェア割り込みが有効になります。３番目のパラメータを`true`にするとPWM出力を有効にします。

{% hint style="info" %}
`change_hz()`で周波数を変更することが出来ます。`change_hz()`では`begin()`の指定より細かい指定が可能です。

`change_duty()`でPWM出力のデューティー比を変更できます。

割り込みハンドラの処理を記述するには、[アプリケーションビヘイビア](../behavior/)の定義が必要です。
{% endhint %}



### end()

```cpp
void end()
```

タイマーの動作を停止します。



### available()

```cpp
inline bool available()
```

タイマー割り込みが発生した直後の`loop()`で`true`になり、`loop()`が終了すれば`false`になります。



### change\_duty()

```cpp
void change_duty(uint16_t duty, uint16_t duty_max = 1024)
```

デューティー比の設定を行う。１番目のパラメータにデューティ比を指定します（小さい値を指定すると波形の平均はGNDレベルに近づき、大きい値を指定するとVccレベルに近づく）。２番目のパラメータはデューティ比の最大値を指定します。

{% hint style="warning" %}
`duty_max`は`1024,4096,16384`のいずれかの指定を推奨します。

内部でのカウント値の計算に除算が発生しますが、これら３つに限りビットシフトによる演算を行っていますが、これ以外の値では計算量の大きい除算処理が実行されます。
{% endhint %}



### change\_hz()

```cpp
void change_hz(uint16_t hz, uint16_t mil = 0) 
```

タイマーの周波数を設定します。２番目のパラメータは周波数の小数点３桁分の値を整数で指定します。例えば 10.4 Hz としたい場合は `hz=10, mil=400` と指定します。



