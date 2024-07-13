# cbAppColdStart()

## Explanation

`cbAppColdStart(bool_t bStart)` is called first on `bStart == FALSE` and then on `bStart == TRUE`. The `bStart == TRUE` clause is used to initialize the application.

This is where the important initialisation of the TWELITE NET (the [`sToCoNet_AppContext`](../gou-zao-ti/stoconet_appcontext.md) structure) needs to be done. Some features can only be configured here.

User-defined event processing functions are registered with the [`ToCoNet_Event_Register_State_Machine`](../yzaibento/toconet_event-api/toconet_event_register_state_machine.md)`()` function.

At the end of this process, the [`ToCoNet_vMacStart()`](../twelite-net-guan-shu/toconet_vmacstart.md) function is called.

The start of the MAC layer is explicitly stated as a procedure in the API.

## Arguments

| Type     | Name     | Remark                                                                              |
| -------- | -------- | ----------------------------------------------------------------------------------- |
| `bool_t` | `bStart` | It is called twice at startup, the first time as FALSE and the second time as TRUE. |

#### bStart

`FALSE`: It is just after the start of microcomputer, and initialization of peripherals etc. has not been done. Call the [`ToCoNet_REG_MOD_ALL()`](../twelite-net-makuro/toconet_reg_mod_all.md) macro of module registration here.

`TRUE`: TWELITE NET Called after the library and hardware APIs have been initialized, but before the library is initialized. The following procedure is described here.

* Setting initial parameters for [`sToCoNet_AppContext`](../gou-zao-ti/stoconet_appcontext.md) structure
* Resister a state machine by [`ToCoNet_Event_Register_State_Machine()`](../yzaibento/toconet_event-api/toconet_event_register_state_machine.md) 
* Initialisation of the application's own memory area etc.
* Hardware initialisation
* Starts MAC layer by[`ToCoNet_vMacStart()`](../twelite-net-guan-shu/toconet_vmacstart.md) 

## Returns

None

## Sample code

```c
void cbAppColdStart(bool_t bAfterAhiInit)
{
	if (!bAfterAhiInit) {
		// before AHI init, very first of code.

		// Register modules
		ToCoNet_REG_MOD_ALL();

	} else {
		// disable brown out detect
		vAHI_BrownOutConfigure(0,//0:2.0V 1:2.3V
				FALSE,
				FALSE,
				FALSE,
				FALSE);

		// clear application context
		memset (&sAppData, 0x00, sizeof(sAppData));
		sAppData.u8channel = CHANNEL;

		// ToCoNet configuration
		sToCoNet_AppContext.u32AppId = APP_ID;
		sToCoNet_AppContext.u8Channel = CHANNEL;

		sToCoNet_AppContext.bRxOnIdle = TRUE;

		// others
		SPRINTF_vInit128();

		// Register
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);

		// Others
		vInitHardware(FALSE);

		// MAC start
		ToCoNet_vMacStart();
	}
}
```

