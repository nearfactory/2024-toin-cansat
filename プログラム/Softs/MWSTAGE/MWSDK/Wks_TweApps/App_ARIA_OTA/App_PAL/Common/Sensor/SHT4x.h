/* Copyright (C) 2021 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

#ifndef  HUM_SHTC3_INCLUDED
#define  HUM_SHTC3_INCLUDED

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "sensor_driver.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define SHT4x_IDX_TEMP 0
#define SHT4x_IDX_HUMID 1
#define SHT4x_IDX_BEGIN 0
#define SHT4x_IDX_END (SHT4x_IDX_HUMID+1) // should be (last idx + 1)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	// data
	int16 ai16Result[2];

	// working
	uint8 u8TickCount, u8TickWait;
	uint8 u8IdxMeasuruing;
} tsObjData_SHT4x;

/****************************************************************************/
/***        Exported Functions (state machine)                            ***/
/****************************************************************************/
void vSHT4x_Init(tsObjData_SHT4x *pData, tsSnsObj *pSnsObj);
void vSHT4x_Final(tsObjData_SHT4x *pData, tsSnsObj *pSnsObj);

#define i16SHT4x_GetTemp(pSnsObj) ((tsObjData_SHT4x *)(pSnsObj->pData)->ai16Result[SHT4x_IDX_TEMP])
#define i16SHT4x_GetHumd(pSnsObj) ((tsObjData_SHT4x *)(pSnsObj->pData)->ai16Result[SHT4x_IDX_HUMID])

#define SHT4x_DATA_NOTYET	(-32768)
#define SHT4x_DATA_ERROR	(-32767)

/****************************************************************************/
/***        Exported Functions (primitive funcs)                          ***/
/****************************************************************************/
PUBLIC bool_t bSHT4xreset();
PUBLIC bool_t bSHT4xstartRead();
PUBLIC int16 i16SHT4xreadResult(int16*, int16*);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif  /* HUM_SHT4x_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

