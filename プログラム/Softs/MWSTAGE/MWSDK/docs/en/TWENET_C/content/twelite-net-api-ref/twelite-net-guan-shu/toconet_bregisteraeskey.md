# ToCoNet_bRegisterAesKey()

## Explanation

Sets the encryption key to be used during communication.

This process is described in the [`E_EVENT_START_UP`](../yzaibento/ibento.md) event at system startup. It cannot be changed during operation.

{% hint style="warning" %}
It is not possible to encrypt the key itself, so if you want to prevent it from being analysed by storing it in EEPROM or by reading the firmware, you should include an algorithm for keeping the key secret.

For encryption we use AES128, but the details are not public.

A 16-bit checksum is added to the data in the packet as a check for decryption. This reduces the amount of data that can be stored by two bytes, but makes the packet more identifiable.
{% endhint %}

## Arguments

| Type            | Name         | Remark                          |
| --------------- | ------------ | ------------------------------- |
| `uint8 *`       | `pu8Key`     | Pointer to 16 bytes of key data |
| `tsCryptDefs *` | `sCryptDefs` | Reserved (specify NULL)         |

## Returns

| Type     | Remark                                                                 |
| -------- | ---------------------------------------------------------------------- |
| `bool_t` | `TRUE`: The key could be registered. `FALSE`: Key registration failed. |

## Sample code

```c
const char *pu8enckey = "abcd1234VWXY7890";
ToCoNet_bRegisterAesKey(pu8enckey, NULL);
```

