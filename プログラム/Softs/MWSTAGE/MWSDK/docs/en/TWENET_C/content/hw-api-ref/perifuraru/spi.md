# SPI

The SPI bus is used for reading and writing sensors, memory devices, etc. Although it uses more pins than I2C, it is often used for devices that require high-speed transfer.

{% hint style="info" %}
Please refer to the Samp_SPI sample.
{% endhint %}

When using the SPI bus, only enough SPI select pins are allocated for the required device. The pins are allocated in order, starting with the SEISEL0 pin.

The following are some examples of initialization and read/write operations; SPI also allows simultaneous write and read operations.

A common example when acquiring data from a sensor is the procedure of sending a command followed by getting the result.

1. Initialize SPI (start using device)
2. Enable select pin for SPI
3. SPI Transfer 1 (8 bits), intended for transmission.
4. SPI transfer 2 (16 bits), intended for reception.

{% hint style="danger" %}
You should always wait for polling (`while(bAHI_SpiPollBusy());`) on each transfer.
{% endhint %}

```c
#define SLAVE_ENABLE2		(1)			//	Using DIO19 and DIO0
#define CS_DIO19			(0x01)		//	Use the one connected to SPISEL0:DIO19
#define CS_DIO0				(0x02)		//	SPISEL1: Use the one connected to DIO0

    ...
    // SPI の初期化
    vAHI_SpiConfigure(SLAVE_ENABLE2, // Use two SPISEL0,1
					  E_AHI_SPIM_MSB_FIRST,
					  TRUE,
					  TRUE,
					  1,
					  E_AHI_SPIM_INT_DISABLE,
					  E_AHI_SPIM_AUTOSLAVE_DSABL);

	...
    // Transfer 1（8bit write out)
    vAHI_SpiSelect(CS_DIO0); // Enable SPISEL1
    vAHI_SpiStartTransfer(
					7, // 7+1=8 bit transfer
					0x00000012L ); // sends 0x12
    while(bAHI_SpiPollBusy()); // Waiting for read/write by polling
    //uint8 u8val = u8AHI_SpiReadTransfer8();
        // SPIMISO input data can be read.
        // In this example, u8val is not used.

    // Give it some time if necessary.
    vWait(100);
    
    //　Transfer 2 (16-bit read)
    vAHI_SpiStartTransfer(
					15, // transfer 15+1=16 bit
					0x00000000L ); // dummy send data
    while(bAHI_SpiPollBusy()); // Waiting for read/write by polling
    uint16 u16val = u16AHI_SpiReadTransfer16();
        // The read 16bit data is stored to u16val.

    // release the SPISEL
    vAHI_SpiStop();

    ...
    // disable the SPI device.
    vAHI_SpiDisable();
```



## functions

### vAHI_SpiConfigure() 

```c
void vAHI_SpiConfigure(
    uint8 u8SlaveEnable,
    bool_t bLsbFirst,
    bool_t bPolarity,
    bool_t bPhase,
    uint8 u8ClockDivider,
    bool_t bInterruptEnable,
    bool_t bAutoSlaveSelect);
```

This function initializes the SPI device and makes it available, and calls `vAHI_SpiDisable()` to release it.

{% hint style="warning" %}
At least one select pin, SPISEL0, is assigned exclusively to SPI. In other words, when using SPI, SPISEL0 (DIO19) cannot be used for any other purpose.
{% endhint %}

| Name               | Remark                                                                                                                                                             |
| ------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `u8SalveEnable`    | Specifies the chip select pin to be enabled. 0 specifies SPISEL0 to be reserved, 1 specifies SPISEL0,1 to be reserved, and 2 specifies SPISEL0,1,2 to be reserved. |
| `bLsbFirst`        | If `TRUE`, the data starts from LSB (lower bit).                                                                                                                   |
| `bPolarity`        | If `TRUE`, reverse the clock.                                                                                                                                      |
| `bPhase`           | If `TRUE`, use the falling edge.                                                                                                                                   |
| `u8ClockDiviser`   | Specifies the clock frequency, 0...63, where 0 means 16MHz drive, and the rest are divided by `2*u8ClockDiviser`.                                                  |
| `bInterruptEnable` | Generates an interrupt at the end of SPI transfer. (It is not used in the samples.)                                                                                |
| `bAutoSlaveSelect` | If `TRUE`, control of the select pin is set to automatic control only during SPI transfer. If `FALSE`, it is set at the timing of calling `vAHI_SpiSelect()`.      |

{% hint style="info" %}
The values of `bLsbFirst`, `bPolarity`, `bPhase`, and `u8ClockDiviser` should be determined from the datasheet of the device to be connected.
{% endhint %}

#### Relationship between SPI Mode and bPolarity, bPhase.

| SPI Mode | `bPolarity` | `bPhase` |
| -------- | ----------- | -------- |
| 0        | `FALSE`     | `FALSE`  |
| 1        | `FALSE`     | `TRUE`   |
| 2        | `TRUE`      | `FALSE`  |
| 3        | `TRUE`      | `TRUE`   |



### vAHI_SpiSelect()

```c
void vAHI_SpiSelect(uint8 u8SlaveMask);
```

Activate select pins.

| Name          | Remark                                                      |
| ------------- | ----------------------------------------------------------- |
| `u8SalveMask` | `1`: SPISEL0, `2`:SPISEL1, `4`:SPISEL3, `0`: cancel select. |



{% hint style="info" %}
`vAHI_SpiStop()` and `vAHI_SpiSelect(0)` are the same call.
{% endhint %}



### vAHI_SpiStartTransfer()

```c
void vAHI_SpiStartTransfer(uint8 u8CharLen, uint32 u32Out);
```

Start SPI transfer. The transfer starts immediately after this function is called, but the control is returned without waiting for the transfer to finish. It waits for the end of transfer by `bAHI_SpiPollBusy()` described below.

| Name        | Remark                                                                                                                                                                           |
| ----------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `u8CharLen` | Specifies the transfer length. 0...31 can be specified, and the specified number + 1 bit is transferred. For example, for a 16-bit transfer, specify 15.                         |
| `u32Out`    | The bit sequence to be output. For transfers of less than 32 bits, the data is right-justified to the LSB side. (In the case of 8-bit data, the data is stored in bit0 to bit7.) |



### bAHI_SpiPollBusy()

```c
bool_t bAHI_SpiPollBusy(void);
```

Waiting for polling of SPI transfer, while TRUE means SPI transfer is in progress.



### u32AHI_SpiReadTransfer32()

```c
 uint32 u32AHI_SpiReadTransfer32(void);
 uint16 u16AHI_SpiReadTransfer16(void);
 uint8 u8AHI_SpiReadTransfer8(void);
```

Called after the SPI transfer is completed, this function reads the received data (SPIMISO) at the time of transfer. The read data is right justified to the LSB side.
