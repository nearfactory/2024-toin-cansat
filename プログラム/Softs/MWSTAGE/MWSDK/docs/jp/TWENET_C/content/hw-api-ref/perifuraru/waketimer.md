# WakeTimer

WakeTimer は 32KHz (外部オシレータの場合 32768Hz でも可) オシレータにより駆動し、２つのハードウェアカウンタが用意されています。WakeTimer を用いたスリープを実行するには [ToCoNet_vSleep](../../twelite-net-api-ref/twelite-net-guan-shu/toconet_vsleep.md)() を利用します。

カウンタは減算式で、値が 0 になったとき割り込み発生を行い、そのまま減算を続けます。

{% hint style="info" %}
スリープ起床につかった WakeTimer の値を確認すれば、起床後の経過時間を知ることが出来ます。この値を次回の周期起床のタイミングの調整に利用することが出来ます。

[ToCoNet_vSleep](../../twelite-net-api-ref/twelite-net-guan-shu/toconet_vsleep.md)() での周期起床のオプションは、この仕組を利用して次回起床タイミングを調整しています
{% endhint %}

### カウント値と実時間の関係

WakeTimer のカウント値と実時間の計算は以下のようになります。

{% hint style="info" %}
キャリブレーション値は WakeTimer によるスリープを実行する際のカウント補正に利用します。

Tms のカウント数は以下の式で計算できます。

```
Ct = Tms * 32 * 10000 / cal
  Ct  : 補正後のカウント数
  Tms : 時間
  cal : キャリブレーション値
```
{% endhint %}

{% hint style="info" %}
外部オシレータを利用する場合は、以下の計算を行います。

```
Ct = (Tms * 32768) / 1000
  Ct  : 補正後のカウント数
  Tms : 時間
  cal : キャリブレーション値
```
{% endhint %}



##  <a href="yin-shu" id="yin-shu"></a>
