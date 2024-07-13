---
title: Wire (using member function)
author: "Mono Wireless Inc."
description: Wire (using member function)
---

# Wire (using member function)

The method using member functions has a relatively low level of abstraction and follows the general API system as provided by the C library. The procedures for operating the two-wire serial bus are more intuitive.

However, it is necessary to be explicitly aware of the start and end of bus usage.

## Read

### requestFrom()

```cpp
size_type requestFrom(
    uint8_t u8address,
    size_type length,
    bool b_send_stop = true)
```

Reads the specified number of bytes at once. Since the result of reading is stored in a queue, call the `.read()` method immediately afterward until the queue is empty.

| Parameters | Description                                |
| ------------------ | --------------------------------- |
| `u8address`        | I2C address to be read                    |
| `length`           | Number of bytes read                          |
| `b_send_stop=true` | When `true`, the `STOP` bit is set at the end of reading. |
| return type `size_type`   | Number of bytes read. `0` means read failure.          |

#### Code Example

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



## Writing

Writing is performed by the `write()` method after executing `beginTransmission()`. Call `endTranmission()` after a series of writing is finished.

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

Initialize the export transfer. Call `endTransmission()` as soon as the writing process is finished. &#x20;

| Parameters | Description |
| ----------- | -------------- |
| `u8address` | I2C address to be written out |

###

### write(value)

```cpp
size_type write(const value_type value)
```

Writes one byte.

| Parameters | Description |
| --------------- | ------------------ |
| Return value `value` | Bytes to be written.
| Return value `size_type` | Number of bytes written. A value of `0` is an error. | Description



### write(\*value, quantity)

```cpp
size_type write(
  const value_type* value,
  size_type quantity)
```

Writes a byte sequence.

| Parameters | Description |
| --------------- | ---------------- |
| `*value` | the byte sequence to be written |
| Return value `size_type` | Number of bytes to be written.
| Return value `size_type` | Number of bytes written. 0 is an error. | Return value `*value` | number of bytes to be written. 0 is an error.



### endTransmission()

```cpp
uint8_t endTransmission(bool sendStop = true)
```

Processes the end of the export. &#x20;

| Parameters | Description |
| ----------------- | -------------- |
| `sendStop = true` | Issue the STOP bit. | Return value `uint8_tstop = true` | Issue the STOP bit.
| Return value `uint8_t` | 0: Success 4: Failure |

