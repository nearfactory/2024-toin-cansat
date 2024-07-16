# ToCoNet_vDebugLevel()

## Explanation

Change the debug output level of TWELITE NET.

The levels are 0 for no output and up to 5. The higher the level, the more messages will be output.

{% hint style="warning" %}
Initialized by [`ToCoNet_vDebugInit()`](toconet_vdebuginit.md).

Set `TOCONET_DEBUG` to `1` in the [Makefile](../../twelite-sdk-howto/birudonitsuite/makefile-nitsuite.md).
{% endhint %}

{% hint style="danger" %}
The debug messages are intended for internal TWELITE NET development and the format of the output will not be disclosed. We may ask you to provide us with the output in order for us to analyse the problem.
{% endhint %}

## Argument

| Type    | Name    | Remark                                                     |
| ------- | ------- | ---------------------------------------------------------- |
| `uint8` | `u8lvl` | New debug output level. (0: no output, 1. .5: with output) |

## Returns

None.

## Sample code

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

