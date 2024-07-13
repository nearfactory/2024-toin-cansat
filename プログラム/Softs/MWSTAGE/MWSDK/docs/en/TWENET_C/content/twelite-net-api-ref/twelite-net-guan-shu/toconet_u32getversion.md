# ToCoNet_u32GetVersion()

## Explanation

Get the version of the library.

The following three functions are available:

```
ToCoNet_u32GetVersion() .. Core library
ToCoNet_u32GetVersion_LibEx() .. EXT
ToCoNet_u32GetVersion_LibUtils() .. UTILS
```

## Argument

None.

## Returns

| Type     | Remark                                                                                  |
| -------- | --------------------------------------------------------------------------------------- |
| `uint32` | Version number `0x00XXYYZZ` (`XX`:major number `YY`:minor number `ZZ`: variant number). |

## Sample code

```c
uint32 u32ver = ToCoNet_u32GetVersion();
```

