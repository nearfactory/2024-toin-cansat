---
title: Serial
author: "Mono Wireless Inc."
description: UART0 port of TWELITE (mwx::serial_jen)
---

# Serial

Implement [`mwx::stream`](../classes/twe-stream/) and input/output with UART0 of TWELITE.

* The `Serial` object is initialized at system startup with UART0, 115200 bps, and the initialization process is performed in the library. On the user code, it is available from `setup()`.
* The `Serial1` object is provided in the library, but no initialization process is performed; to enable UART1, perform the necessary initialization procedures `Serial1.setup(), Serial1.begin()`.

{% hint style="danger" %}
Output may become unstable during `setup(), wakeup()` just after startup or `flush` process just before sleep.
{% endhint %}

###

### setup()

```cpp
void setup(uint16_t buf_tx, uint16_t buf_rx)
```

Initialize objects.

* Allocate memory for FIFO buffers for TX/RX
* Allocating memory for TWE\_tsFILE structure

{% hint style="warning" %}
`Serial`(UART0) will automatically call `setup()` in the library. There is no need for a user call.

Also, the buffer size of `Serial` (UART0) is determined at compile time. You can change it by the macro `MWX_SER_TX_BUFF` (768 if not specified) or `MWX_SER_RX_BUFF` (256 if not specified).
{% endhint %}

| Parameter | Description |
| -------- | --------------- |
| `buf_tx` | FIFO buffer size for TX |
| `buf_rx` | FIFO buffer size for RX |



### begin()

```cpp
void begin(unsigned long speed = 115200, uint8_t config = 0x06)
```

Initialize hardware.

{% hint style="warning" %}
The `Serial` (UART0) has an automatic `begin()` call in the library. No user call is required.
{% endhint %}

| Parameters | Description |
| -------- | ------------------------------------------------------------------------------------------------- |
| `speed` | Specifies the baud rate of the UART.                                                                                 |
| `config` | When the `serial_jen::E_CONF::PORT_ALT` bit is specified, UART1 is initialized with DIO14,15. If not specified, initializes UART1 with DIO11(TxD),9(RxD). |

{% hint style="info" %}
The last two digits of the specified baud rate are rounded to zero. Also, due to hardware limitations, there may be an error from the specified baud rate.

When specifying 9600, 38400, or 115200, the baud rate is calculated without division. See `constexpr uint16_t _serial_get_hect_baud(uint32_t baud)` for details of processing.
{% endhint %}



### end()

(Not implemented) Stop using hardware.



### get\_tsFile()

```cpp
TWE_tsFILE* get_tsFile();
```

Get a structure in the `TWE_tsFILE*` format used in the C library.

{% hint style="info" %}
In Serial (UART), the `_sSerial` structure is defined.
{% endhint %}

