# ToCoNet_u16RcCalib()

## Explanation <a href="jie" id="jie"></a>

Calibrate the RC timer.

* Performing a calibration
* Getting calibration values
* Setting the calibration values

{% hint style="info" %}
You can also use `ToCoNet_u16GetRcCalib()` to get the calibration values.
{% endhint %}

### Use of external oscillator at 32768Hz

No calibration is required and a fixed value of 9766 (32000/32768) counts is specified. Use [`cbAppColdStart()`](../krubakku/cbappcoldstart.md) as shown in the following example.

```c
// Only valid for TWENET library 1.2.0 and later
void cbAppColdStart(bool_t bAfterAhiInit)
{
	if (!bAfterAhiInit) {
		ToCoNet_REG_MOD_ALL();
	} else {
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);
		...
		// Omit calibration at system start-up
		sToCoNet_AppContext.bSkipBootCalib = TRUE;
		// Setting fixed values (v1.2 and later)
		ToCoNet_u16RcCalib(9766);
	}
}

void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg) {
        
     if (eEvent == E_EVENT_START_UP) { //At system start-up
     	// Setting of fixed values
 	ToCoNet_u16RcCalib(9766);
     }
 }

```

## Argument <a href="yin-shu" id="yin-shu"></a>

| Type      | Name      | Value        | Remark                                                                                   |
| --------- | --------- | ------------ | ---------------------------------------------------------------------------------------- |
| ​`uint16` | `​u16val` | `0`          | Calibration is carried out. Calibration takes less than 1 ms, but some time is required. |
|           |           | `5000-15000` | Sets the calibration value.                                                              |
|           |           | `0xFFFF`     | Retrieves the set value.                                                                 |



## Returns <a href="ri" id="ri"></a>

| Type      | Remark                                                                                           |
| --------- | ------------------------------------------------------------------------------------------------ |
| `​uint16` | If the argument `u16val == 0xFFFF`, the current setting value is returned. Otherwise undefined.​ |



## Sample Code

```c
vfPrintf(&sSerStream, "RC Calib = %d"LB,  ToCoNet_u16GetRcCalib());
```

