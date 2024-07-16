# Wiring for firmware programming

This section describes the wiring for programming the firmware.

{% hint style="info" %}
For more information on TWELITE R2 compatible wiring, please refer to "[General Information > Technical Information > Wiring for firmware programming](fumuua.md#pinnitsuite)".
{% endhint %}

{% hint style="warning" %}
If you are designing your own board with a surface mount module (SMD) version, please make sure that there are wires on the board that allow you to rewrite the firmware.
{% endhint %}



## Pinning 

The wires used in the program mode are listed below. These signal lines can be accessed externally.

| **Name** | **Signal name** | **TWELITE DIP#**                          | **TWELITE SMD #**                        | **Remarks**                                                                                                                        |
| -------- | --------------- | ----------------------------------------- | ---------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| RST      | RESETN          | 21                                        | 21                                       | Reset the module (if not wired, can be replaced by power-on)                                                                       |
| RXD      | DIO7            | 3                                         | 9                                        | Serial communication line (PC side connected to TX)                                                                                |
| PRG      | SPIMISO         | 7                                         | 2                                        | When this pin is set to LO (GND) and then reset and released (or HI), the device transitions to program mode.                      |
| TXD      | DIO6            | 10                                        | 8                                        | Serial communication line (PC side is connected to RX)                                                                             |
| GND      | GND             | 1,14                                      | 20,28                                    | For power supply (several pins are defined in TWELITE, which are connected internally, so a single connection can be acceptable)   |
|          |                 |                                           |                                          |                                                                                                                                    |
|          | SET             | Depends on the mother board (normally 15) | Depends on the mother board normally 13) | On the TWELITE Stage, it is set to SET=LO immediately after reset to display the interactive mode screen in sleep driven firmware. |
| VCC      | VCC             | 28                                        | 5                                        | For power supply                                                                                                                   |



{% hint style="warning" %}
When connected to TWELITE R/2, the same signal is wired directly to the TWELITE  microcontroller and also supplies VCC, which can be delivered from the output of TWELITE R/2.

First of all, please make sure that no other circuits are connected to the relevant signal lines and that no signal lines not listed above are connected. Other circuits may affect the transition to the program mode or cause problems with the serial communication during programming.
{% endhint %}

{% hint style="info" %}
The TWELITE R/2 allows for the connection of 2.54mm pitch header pins and sockets in the above order RST RXD PRG TXD GND. If you have the same arrangement on your board, you can use TWELITE R.
{% endhint %}

## Program mode setting procedure

| STEP | Procedure                                                                                                                        |
| ---- | -------------------------------------------------------------------------------------------------------------------------------- |
| 1    | Set the TWELITE module powered off.                                                                                              |
| 2    | Connect the PRG (SPIMISO) pin to GND.                                                                                            |
| 3    | Turn the power on. (or reset)                                                                                                    |
| 4    | Leave the PRG (SPIMISO) pin unconnected. (Internal pull-up makes the pin state Hi.)                                              |
|      | _At this point the TWELITE radio module is running in program mode with rewritable firmware._                                    |
| 5    | Start the firmware programmer on the PC and check the connection to the target serial port.                                      |
|      | _If there is no connection response (no serial number etc. displayed, no response and timeout), start again from the beginning._ |

{% hint style="info" %}
In combinations, such as TWELITE R2 and TWELITE Stage, steps 1 to 5 above are automated.
{% endhint %}



## Wiring example (control by push button switch)

The wiring is as follows: the PRG and RST pins are connected to a pushbutton switch, which drops to GND when the button is pressed.

![配線例](<../../.gitbook/assets/image (33).png>)

Control the PGM and RST pins as follows: Immediately after releasing the button on the PGM pin, the TWELITE radio module will enter program mode.

![](<../../.gitbook/assets/image (34).png>)

{% hint style="warning" %}
This may not work due to chattering switches etc. If the TWELITE wireless microcontroller does not respond, please try again.

A short waiting time of around 100 ms is sufficient.
{% endhint %}
