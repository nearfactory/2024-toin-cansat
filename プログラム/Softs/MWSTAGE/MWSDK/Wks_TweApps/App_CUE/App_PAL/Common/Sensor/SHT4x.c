/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "AppHardwareApi.h"
#include "utils.h"
#include "string.h"

#include "sensor_driver.h"
#include "SHT4x.h"
#include "SMBus.h"

#include "ccitt8.h"

#undef SERIAL_DEBUG
#ifdef SERIAL_DEBUG
# include <serial.h>
# include <fprintf.h>
extern tsFILE sDebugStream;
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define SHT4x_ADDRESS	(0x44)
//#define SHT4x_ADDRESS	(0x45)

#define SHT4x_MSR_HIGH_PRECISION (0xFD)
#define SHT4x_MSR_MEDIUM_PRECISION (0xF6)
#define SHT4x_MSR_LOW_PRECISION (0xE0)

#define SHT4x_READ_SERIAL (0x89)
#define SHT4x_RESET (0x94)

#define SHT4x_ACTIVATE_HEATER_HIGH_POWER_LONG (0x39)
#define SHT4x_ACTIVATE_HEATER_HIGH_POWER_SHORT (0x32)
#define SHT4x_ACTIVATE_HEATER_MEDIUM_POWER_LONG (0x2F)
#define SHT4x_ACTIVATE_HEATER_MEDIUM_POWER_SHORT (0x24)
#define SHT4x_ACTIVATE_HEATER_LOW_POWER_LONG (0x1E)
#define SHT4x_ACTIVATE_HEATER_LOW_POWER_SHORT (0x15)

#define SHT4x_CONVTIME_HIGH_PRECISION		(9) // 9ms
#define SHT4x_CONVTIME_MEDIUM_PRECISION		(5) // 5ms
#define SHT4x_CONVTIME_LOW_PRECISION		(2) // 2ms

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vProcessSnsObj_SHT4x(void *pvObj, teEvent eEvent);
PRIVATE uint8 u8CRC8( uint8* u8buf, uint8 u8len);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
void vSHT4x_Init(tsObjData_SHT4x *pData, tsSnsObj *pSnsObj) {
	vSnsObj_Init(pSnsObj);

	pSnsObj->pvData = (void*)pData;
	pSnsObj->pvProcessSnsObj = (void*)vProcessSnsObj_SHT4x;

	memset((void*)pData, 0, sizeof(tsObjData_SHT4x));
}

void vSHT4x_Final(tsObjData_SHT4x *pData, tsSnsObj *pSnsObj) {
	pSnsObj->u8State = E_SNSOBJ_STATE_INACTIVE;
}

/****************************************************************************
 *
 * NAME: vSHT4xreset
 *
 * DESCRIPTION:
 *   to reset SHT4x device
 *
 * RETURNS:
 * bool_t	fail or success
 *
 ****************************************************************************/
PUBLIC bool_t bSHT4xreset()
{
	return bSMBusWrite(SHT4x_ADDRESS, SHT4x_RESET, 0, NULL );
}

/****************************************************************************
 *
 * NAME: vHTSstartReadTemp
 *
 * DESCRIPTION:
 * Wrapper to start a read of the temperature sensor.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC bool_t bSHT4xstartRead()
{
	// start conversion (will take some ms according to bits accuracy)
//	return bSMBusWrite(SHT4x_ADDRESS, SHT4x_MSR_LOW_PRECISION, 0, NULL );
	return bSMBusWrite(SHT4x_ADDRESS, SHT4x_MSR_MEDIUM_PRECISION, 0, NULL );
//	return bSMBusWrite(SHT4x_ADDRESS, SHT4x_MSR_HIGH_PRECISION, 0, NULL );
}

/****************************************************************************
 *
 * NAME: u16SHT4xreadResult
 *
 * DESCRIPTION:
 * Wrapper to read a measurement, followed by a conversion function to work
 * out the value in degrees Celcius.
 *
 * RETURNS:
 * int16: temperature in degrees Celcius x 100 (-4685 to 12886)
 *        0x8000, error
 *
 * NOTES:
 * the data conversion fomula is :
 *      TEMP:  -46.85+175.72*ReadValue/65536
 *      HUMID: -6+125*ReadValue/65536
 *
 *    where the 14bit ReadValue is scaled up to 16bit
 *
 ****************************************************************************/
PUBLIC int16 i16SHT4xreadResult(int16 *pi16Temp, int16 *pi16Humid)
{
	bool_t bOk = TRUE;
	uint16 u16temp_val;
	uint16 u16humid_val;
	uint8 au8data[6];

	bOk &= bSMBusSequentialRead(SHT4x_ADDRESS, 6, au8data);
	if(!bOk) return SHT4x_DATA_NOTYET; // error

	// CRC8 check
	uint8 u8crc = u8CRC8(au8data, 2);
	if (au8data[2] != u8crc) return SHT4x_DATA_ERROR;

	u8crc = u8CRC8(au8data+3,2);
	if (au8data[5] != u8crc) return SHT4x_DATA_ERROR;

 	u16temp_val = au8data[1] + (au8data[0] << 8);
	if(pi16Temp){
		*pi16Temp = (int16)(((17500UL*u16temp_val)>>16)-4500);
//		*pi16Temp = (int16)(((17500*u16temp_val)/65535)-4500);		// 正確にはこの計算式
		if(*pi16Temp > 13000 || -4500 > *pi16Temp ) *pi16Temp = SHT4x_DATA_ERROR;
	}
	else return SHT4x_DATA_ERROR;

	u16humid_val = au8data[4] + (au8data[3] << 8);
	if(pi16Humid){
		*pi16Humid = (int16)( ((12500UL*u16humid_val)>>16)-600 );
//		*pi16Humid = (int16)( ((12500*u16humid_val)/65535)-600 );	// 正確にはこの計算式
		if(*pi16Humid > 10000) *pi16Humid = 10000;
		if(*pi16Humid < 0) *pi16Humid = 0;
	}
	else return SHT4x_DATA_ERROR;

	return 0;
}

uint8 u8CRC8( uint8* buf, uint8 u8len )
{
	uint8 u8crc = 0xFF;
	uint8 u8GP = 0x31;	// X8+X5+X4+1
	uint8 i = 0;
	uint8 j = 0;

	for( i=0; i<u8len; i++ ){
		u8crc ^= buf[i];
		for( j=0;j<8;j++ ){
			if( u8crc&0x80 ){
				u8crc <<= 1;
				u8crc ^= u8GP;
			}else{
				u8crc <<= 1;
			}
		}
	}
	return u8crc;
}
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
// the Main loop
void vProcessSnsObj_SHT4x(void *pvObj, teEvent eEvent) {
	tsSnsObj *pSnsObj = (tsSnsObj *)pvObj;
	tsObjData_SHT4x *pObj = (tsObjData_SHT4x *)pSnsObj->pvData;

	// general process
	switch (eEvent) {
		case E_EVENT_TICK_TIMER:
			if (pObj->u8TickCount < 100) {
				pObj->u8TickCount += pSnsObj->u8TickDelta;
#ifdef SERIAL_DEBUG
vfPrintf(&sDebugStream, "+");
#endif
			}
			break;
		case E_EVENT_START_UP:
			pObj->u8TickCount = 100; // expire immediately
#ifdef SERIAL_DEBUG
vfPrintf(&sDebugStream, "\n\rSHT4x WAKEUP");
#endif
		break;

		default:
			break;
	}

	// state machine
	switch(pSnsObj->u8State)
	{
	case E_SNSOBJ_STATE_INACTIVE:
		// do nothing until E_ORDER_INITIALIZE event
		break;

	case E_SNSOBJ_STATE_IDLE:
		switch (eEvent) {
		case E_EVENT_NEW_STATE:
			break;

		case E_ORDER_KICK:
			vSnsObj_NewState(pSnsObj, E_SNSOBJ_STATE_MEASURING);

			#ifdef SERIAL_DEBUG
			vfPrintf(&sDebugStream, "\n\rSHT4x KICKED");
			#endif
			break;

		default:
			break;
		}
		break;

	case E_SNSOBJ_STATE_MEASURING:
		switch (eEvent) {
		case E_EVENT_NEW_STATE:
			pObj->ai16Result[SHT4x_IDX_TEMP] = SENSOR_TAG_DATA_ERROR;
			pObj->ai16Result[SHT4x_IDX_HUMID] = SENSOR_TAG_DATA_ERROR;
			//pObj->u8TickWait = SHT4x_CONVTIME_LOW_PRECISION;
			pObj->u8TickWait = SHT4x_CONVTIME_MEDIUM_PRECISION;
			//pObj->u8TickWait = SHT4x_CONVTIME_HIGH_PRECISION;

			// kick I2C communication
			if (!bSHT4xstartRead()) {
				vSnsObj_NewState(pSnsObj, E_SNSOBJ_STATE_COMPLETE); // error
			}

			pObj->u8TickCount = 0;
			break;

		default:
			break;
		}

		// wait until completion
		if (pObj->u8TickCount > pObj->u8TickWait) {
			int16 i16ret;
			i16ret = i16SHT4xreadResult(
					&(pObj->ai16Result[SHT4x_IDX_TEMP]),
					&(pObj->ai16Result[SHT4x_IDX_HUMID]) );

			if (i16ret == SENSOR_TAG_DATA_ERROR) {
				vSnsObj_NewState(pSnsObj, E_SNSOBJ_STATE_COMPLETE); // error
			} else
			if (i16ret == SENSOR_TAG_DATA_NOTYET) {
				// still conversion
				#ifdef SERIAL_DEBUG
				vfPrintf(&sDebugStream, "\r\nSHT_ND");
				#endif

				pObj->u8TickCount /= 2; // wait more...
			} else {
				// data arrival
				vSnsObj_NewState(pSnsObj, E_SNSOBJ_STATE_COMPLETE);
			}
		}
		break;

	case E_SNSOBJ_STATE_COMPLETE:
		switch (eEvent) {
		case E_EVENT_NEW_STATE:
			#ifdef SERIAL_DEBUG
			vfPrintf(&sDebugStream, "\n\rSHT_CP: T%d H%d",
					pObj->ai16Result[SHT4x_IDX_TEMP],
					pObj->ai16Result[SHT4x_IDX_HUMID]);
			#endif

			break;

		case E_ORDER_KICK:
			// back to IDLE state
			vSnsObj_NewState(pSnsObj, E_SNSOBJ_STATE_IDLE);
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
