/* Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.    *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE   *
 * AGREEMENT).                                                   */

#include <jendefs.h>

#include "utils.h"
#include "ccitt8.h"
#include "Interactive.h"
#include "sensor_driver.h"
#include "MC3630.h"

#include "flash.h"

#ifdef USE_CUE
#include "App_CUE.h"
#elif USE_ARIA
#include "App_ARIA.h"
#else
#include "EndDevice.h"
#endif

static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg);
static void vStoreSensorValue();
static void vProcessMC3630(teEvent eEvent);
static bool_t bSendData( int16 ai16accel[3][32], uint8 u8startAddr, uint8 u8Sample );
static bool_t bSendAppTag( int16 ai16accel[3][32], uint8 u8startAddr, uint8 u8Sample );
static void vCalc_Average_MinMax(int16 ai16accel[3][32], uint8 u8Sample);

#define ABS(c) (c<0?(-1*c):c)

static uint8 u8sns_cmplt = 0;
tsObjData_MC3630 sObjMC3630;

static uint8 u8ConMax = 0;
static uint8 u8FIFOCount = 0;
static bool_t bContinuous = FALSE;
static bool_t bActive = FALSE;
//static bool_t bEvent = FALSE;
static uint16 u16Threshold = 0;
static bool_t bShortSleep = FALSE;

uint64 u64BlinkTimer = 0;

enum {
	E_SNS_ADC_CMP_MASK = 1,
	E_SNS_MC3630_CMP = 2,
	E_SNS_ALL_CMP = 3
};

int32 ai32ave[3] = {0, 0, 0};
int16 ai16max[3] = {-20000, -20000, -20000};
int16 ai16min[3] = {20000, 20000, 20000};
uint16 u16SamplingCount = 0;
uint16 Frequency = 0;

uint32 u32StartTime = 0;
uint32 u32EndTime = 0;

/*
 * ADC 計測をしてデータ送信するアプリケーション制御
 */
PRSEV_HANDLER_DEF(E_STATE_IDLE, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	static bool_t bFirst = TRUE;
	if (eEvent == E_EVENT_START_UP) {
		if (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK) {
			// Warm start message
			V_PRINTF(LB "*** Warm starting woke by %s. ***", sAppData.bWakeupByButton ? "DIO" : "WakeTimer");
		} else {
			// 開始する
			// start up message
			vSerInitMessage();

			V_PRINTF(LB "*** Cold starting");
			V_PRINTF(LB "* start end device[%d]", u32TickCount_ms & 0xFFFF);
		}

		// RC クロックのキャリブレーションを行う
		ToCoNet_u16RcCalib(sAppData.sFlash.sData.u16RcClock);
		V_PRINTF(LB "* calib[%d]", sAppData.sFlash.sData.u16RcClock);

		// 定期送信するときはカウントが0の時と割り込み起床したときだけ送信する
		if( sAppData.u32SleepCount != 0 && sAppData.bWakeupByButton == FALSE ){
			V_PRINTF(LB"*** No Send...");
			ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP);
			return;
		}

		// センサーがらみの変数の初期化
		u8sns_cmplt = 0;
		vMC3630_Init( &sObjMC3630, &sSnsObj );

		bShortSleep = FALSE;

		if( bFirst ){
			bFirst = FALSE;
			ToCoNet_Event_SetState(pEv, E_STATE_APP_BLINK_LED);
		}else if(IS_APPCONF_OPT_ONESHOTMODE()){
				V_PRINTF(LB "*** MC3630 Start Measurement(One Shot) ");
				vMC3630_Wakeup();

				vSnsObj_Process(&sSnsObj, E_ORDER_KICK);
				if (bSnsObj_isComplete(&sSnsObj)) {
					// 即座に完了した時はセンサーが接続されていない、通信エラー等
					u8sns_cmplt |= E_SNS_MC3630_CMP;
					V_PRINTF(LB "*** MC3630 comm err?");
					ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
					return;
				}

				// ADC の取得
				vADC_WaitInit();
				vSnsObj_Process(&sAppData.sADC, E_ORDER_KICK);

				ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
		}else if( !bContinuous && !sAppData.bWakeupByButton ){
			if(u8FIFOCount == 0){
				u8FIFOCount = u8ConMax;
				V_PRINTF(LB "*** MC3630 Start Measurement ");
				u32StartTime = U64_LOWER_U32(u64GetTimer_ms());
				vMC3630_StartFIFO();
			}
			bShortSleep = TRUE;
			ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP);
		}else{
			if( !bMC3630_IsActive() ){
				if(u8FIFOCount == 0){
					u8FIFOCount = u8ConMax;
					V_PRINTF(LB "*** MC3630 Start Measurement ");
					u32StartTime = U64_LOWER_U32(u64GetTimer_ms());
					vMC3630_StartFIFO();
				}
				bShortSleep = TRUE;
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP);
			}else{
				u32EndTime = U64_LOWER_U32(u64GetTimer_ms());
				vSnsObj_Process(&sSnsObj, E_ORDER_KICK);
				if (bSnsObj_isComplete(&sSnsObj)) {
					// 即座に完了した時はセンサーが接続されていない、通信エラー等
					u8sns_cmplt |= E_SNS_MC3630_CMP;
					V_PRINTF(LB "*** MC3630 comm err?");
					ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
					return;
				}

				sObjMC3630.u8Interrupt = u8MC3630_ReadInterrupt();
				if( (sObjMC3630.u8Interrupt&0x44) == 0 ){
					ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP);
					return;
				}

				// ADC の取得
				vADC_WaitInit();
				vSnsObj_Process(&sAppData.sADC, E_ORDER_KICK);

				// センサ値を読み込んで送信する
				// RUNNING 状態
				ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
			}
		}

	} else {
		V_PRINTF(LB "*** unexpected state.");
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

/* 電源投入時はLEDを点滅させる */
PRSEV_HANDLER_DEF(E_STATE_APP_BLINK_LED, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	if(eEvent == E_EVENT_NEW_STATE){
		V_PRINTF(LB "*** MC3630 Setting...");

		uint8 mode  = (sAppData.sFlash.sData.u32param>>21)&0x03;
		uint8 freqid = (sAppData.sFlash.sData.u32param>>8)&0x0F;
		V_PRINTF( LB"mode=%d, freqid=0x%X", mode, freqid );
		sObjMC3630.u8SampleFreq = au8SplFreq[mode][freqid];

		if( IS_APPCONF_OPT_ONESHOTMODE() ){
			V_PRINTF(LB "*** Oneshot Mode!");
			bool_t bOk = bMC3630_reset( sObjMC3630.u8SampleFreq, MC3630_RANGE16G, 1 );

			if(bOk == FALSE){
				V_PRINTF(LB "Access failed.");
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
				return;
			}

			vMC3630_Sleep();

			V_PRINTF(LB "*** MC3630 Setting Compleate!");
			V_PRINTF(LB "*** Blink LED ");
			sAppData.u8LedState = 0x02;

		}else{
			if(sAppData.sFlash.sData.u32Slp == 0 ){
				bContinuous = TRUE;
				V_PRINTF( LB"!Continuous" );
			}

			u16Threshold = (sAppData.sFlash.sData.u32param>>12)&0x0F;
			if( u16Threshold != 0 && bContinuous ){
				u8FIFOCount = 0;
				u16Threshold *= 1000;
				bActive = TRUE;
				V_PRINTF( LB"!Active" );
			}
			V_PRINTF(LB"TH = %d", u16Threshold);


			uint8 u8SampleNum = 16;
			if( IS_APPCONF_OPT_2525AMODE() ){
				u8SampleNum = 10;
			}else if(bActive){
				u8SampleNum = 30;
			}

			bool_t bOk = TRUE;
			// 連続モードではない時に連続で送る回数
			u8ConMax = (sAppData.sFlash.sData.u32param&0xFF) + 1;

			bOk = bMC3630_reset( sObjMC3630.u8SampleFreq, MC3630_RANGE16G, u8SampleNum );
			if(bOk == FALSE){
				V_PRINTF(LB "Access failed.");
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
				return;
			}

			V_PRINTF(LB "*** MC3630 Setting Compleate!");
			V_PRINTF(LB "*** Blink LED ");
			sAppData.u8LedState = 0x02;
		}
	}

	// タイムアウトの場合はスリープする
	if (ToCoNet_Event_u32TickFrNewState(pEv) > 750) {
		V_PRINTF(LB "*** MC3630 First Sleep ");

		if(!IS_APPCONF_OPT_ONESHOTMODE()){
			vMC3630_StartFIFO();
			u32StartTime = U64_LOWER_U32(u64GetTimer_ms());
		}
		sAppData.u8LedState = 0;
		LED_OFF();
		if(!bActive) u8FIFOCount = u8ConMax;
		if(!bContinuous) bShortSleep = TRUE;
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

PRSEV_HANDLER_DEF(E_STATE_RUNNING, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
		// 短期間スリープからの起床をしたので、センサーの値をとる
	if ((eEvent == E_EVENT_START_UP) && (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK)) {
		V_PRINTF("#");
		vProcessMC3630(E_EVENT_START_UP);
	}

	// 送信処理に移行
	if (u8sns_cmplt == E_SNS_ALL_CMP) {
		if( IS_APPCONF_OPT_ONESHOTMODE() ){
			vMC3630_Sleep();
			ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
		}

		if(!bContinuous && u8FIFOCount == 1){
			vMC3630_Sleep();
			V_PRINTF(LB"! STOP MC3630");
		}

		if(IS_APPCONF_OPT_SEND_EXTRADATA() && !IS_APPCONF_OPT_AVERAGEMODE() ){
			u16SamplingCount += sObjMC3630.u8FIFOSample;
		}

		if(IS_APPCONF_OPT_AVERAGEMODE()){
			V_PRINTF(LB"FIFO Count = %d", u8FIFOCount);
			V_PRINTF(LB"Continuous = %s", bContinuous ? "TRUE":"FALSE");
			vCalc_Average_MinMax( sObjMC3630.ai16Result, sObjMC3630.u8FIFOSample );
			if( !bContinuous && u8FIFOCount != 1 ){
				V_PRINTF(LB"! Sleep (E_STATE_RUNNING)");
				u8FIFOCount--;
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
				return;
			}
		}

		if(bActive){
			ToCoNet_Event_SetState(pEv, E_STATE_APP_JUDGE_SEND);
		}else{
			ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
		}
	}

	// タイムアウト
	if (ToCoNet_Event_u32TickFrNewState(pEv) > 100) {
		if( IS_APPCONF_OPT_ONESHOTMODE() ){
			vMC3630_Sleep();
		}
		V_PRINTF(LB"! TIME OUT (E_STATE_RUNNING)");
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

PRSEV_HANDLER_DEF(E_STATE_APP_JUDGE_SEND, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	static bool_t bSend = FALSE;
	uint16 u16FalseNum = 0;
	static uint8 u8sendCounter = 0;
	static int16 ai16beforeAccel[3][32];
	static uint8 u8beforeSmplNum = 0;
	if (eEvent == E_EVENT_NEW_STATE) {
		u8sendCounter = 0;
		uint8 i, j;
		for( i=0; i<sObjMC3630.u8FIFOSample; i++ ){
			for( j=0; j<3; j++ ){
				if( ABS(sObjMC3630.ai16Result[j][i]) > u16Threshold ){
					bSend = TRUE;
					u8FIFOCount = u8ConMax;
				}else{
					u16FalseNum++;
				}
			}
		}

		V_PRINTF(LB"bSend = %d", bSend);
		V_PRINTF(LB"u8FIFOCount = %d", u8FIFOCount);

		if( bSend || u8FIFOCount > 0 ){
			// ネットワークの初期化
			if (!sAppData.pContextNwk) {
				// 初回のみ
				sAppData.sNwkLayerTreeConfig.u8Role = TOCONET_NWK_ROLE_ENDDEVICE;
				sAppData.pContextNwk = ToCoNet_NwkLyTr_psConfig_MiniNodes(&sAppData.sNwkLayerTreeConfig);
				if (sAppData.pContextNwk) {
					ToCoNet_Nwk_bInit(sAppData.pContextNwk);
					ToCoNet_Nwk_bStart(sAppData.pContextNwk);
				} else {
					ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
					return;
				}
			} else {
				// 一度初期化したら RESUME
				ToCoNet_Nwk_bResume(sAppData.pContextNwk);
			}

			bool_t bsendFalse = FALSE;
			if ( bSendData( sObjMC3630.ai16Result, 0, 16 ) ) {
				ToCoNet_Tx_vProcessQueue(); // 送信処理をタイマーを待たずに実行する
			}else{
				bsendFalse = TRUE;
			}

			if ( bSendData( sObjMC3630.ai16Result, 16, u8beforeSmplNum-16 ) ) {
				ToCoNet_Tx_vProcessQueue(); // 送信処理をタイマーを待たずに実行する
			} else {
				bsendFalse = TRUE;
			}

			if(bsendFalse){
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // 送信失敗
			}

			V_PRINTF(" FR=%04X", sAppData.u16frame_count);
		}else{
			// データをコピーする
			for(i=0;i<3;i++){
				memcpy( ai16beforeAccel[i], sObjMC3630.ai16Result[i], sObjMC3630.u8FIFOSample );
			}
			u8beforeSmplNum = sObjMC3630.u8FIFOSample;

			ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
		}

		if(u8FIFOCount > 0){
			u8FIFOCount--;
		}
	}

	if( u16FalseNum == (sObjMC3630.u8FIFOSample*3) ){
		bSend = FALSE;
	}

	if (eEvent == E_ORDER_KICK) { // 送信完了イベントが来たのでスリープする
		u8sendCounter++;
		if(u8sendCounter == 2){
			// データをコピーする
			uint8 i;
			for(i=0;i<3;i++){
				memcpy( ai16beforeAccel[i], sObjMC3630.ai16Result[i], sObjMC3630.u8FIFOSample );
			}
			u8beforeSmplNum = sObjMC3630.u8FIFOSample;

			ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // 寝る
		}
	}

	// タイムアウト
	if (ToCoNet_Event_u32TickFrNewState(pEv) > 100) {
		// データをコピーする
		uint8 i;
		for(i=0;i<3;i++){
			memcpy( ai16beforeAccel[i], sObjMC3630.ai16Result[i], sObjMC3630.u8FIFOSample );
		}
		u8beforeSmplNum = sObjMC3630.u8FIFOSample;

		V_PRINTF(LB"! TIME OUT (E_STATE_APP_WAIT_TX)");
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

PRSEV_HANDLER_DEF(E_STATE_APP_WAIT_TX, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	if (eEvent == E_EVENT_NEW_STATE) {
		// ネットワークの初期化
		if (!sAppData.pContextNwk) {
			// 初回のみ
			sAppData.sNwkLayerTreeConfig.u8Role = TOCONET_NWK_ROLE_ENDDEVICE;
			sAppData.pContextNwk = ToCoNet_NwkLyTr_psConfig_MiniNodes(&sAppData.sNwkLayerTreeConfig);
			if (sAppData.pContextNwk) {
				ToCoNet_Nwk_bInit(sAppData.pContextNwk);
				ToCoNet_Nwk_bStart(sAppData.pContextNwk);
			} else {
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
				return;
			}
		} else {
			// 一度初期化したら RESUME
			ToCoNet_Nwk_bResume(sAppData.pContextNwk);
		}


		if( sObjMC3630.u8FIFOSample > 16 ){
			if ( bSendData( sObjMC3630.ai16Result, 0, 16 ) ) {
				ToCoNet_Tx_vProcessQueue(); // 送信処理をタイマーを待たずに実行する
			} else {
				//ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // 送信失敗
			}
			V_PRINTF(" FR=%04X", sAppData.u16frame_count);
			if ( bSendData( sObjMC3630.ai16Result, 16, sObjMC3630.u8FIFOSample-16 ) ) {
				ToCoNet_Tx_vProcessQueue(); // 送信処理をタイマーを待たずに実行する
			} else {
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // 送信失敗
			}
			V_PRINTF(" FR=%04X", sAppData.u16frame_count);
		}else{
			if ( bSendData( sObjMC3630.ai16Result, 0, sObjMC3630.u8FIFOSample ) ) {
				ToCoNet_Tx_vProcessQueue(); // 送信処理をタイマーを待たずに実行する
			} else {
				ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // 送信失敗
			}
			V_PRINTF(" FR=%04X", sAppData.u16frame_count);
		}
	}

	if (eEvent == E_ORDER_KICK) { // 送信完了イベントが来たのでスリープする
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // 寝る
	}

	// タイムアウト
	if (ToCoNet_Event_u32TickFrNewState(pEv) > 100) {
		V_PRINTF(LB"! TIME OUT (E_STATE_APP_WAIT_TX)");
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

PRSEV_HANDLER_DEF(E_STATE_APP_SLEEP, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	if (eEvent == E_EVENT_NEW_STATE) {
		// スリープ時間を計算する
		uint32 u32Sleep = 60000;	// 60 * 1000ms
		if( sAppData.u32SleepCount == 0 && sAppData.u8Sleep_sec ){
			u32Sleep = sAppData.u8Sleep_sec*1000;
		}

		V_PRINTF(LB"! Sleeping... %d %c", u32Sleep, bShortSleep ? 's':' ' );
		V_FLUSH();

		// 平均モードでない場合は、割り込みが入った時間をサンプリング開始時間とする。
		if( (!IS_APPCONF_OPT_AVERAGEMODE() && !bShortSleep) || bContinuous ){
			u32StartTime = u32EndTime;
		}

		// Sleep は必ず E_EVENT_NEW_STATE 内など１回のみ呼び出される場所で呼び出す。
		// Mininode の場合、特別な処理は無いのだが、ポーズ処理を行う
		if (sAppData.pContextNwk) {
			ToCoNet_Nwk_bPause(sAppData.pContextNwk);
		}

		pEv->bKeepStateOnSetAll = FALSE; // スリープ復帰の状態を維持しない

		vAHI_UartDisable(UART_PORT); // UART を解除してから(このコードは入っていなくても動作は同じ)

//		vAHI_DioSetDirection( u32DioPortWakeUp, 0); // set as input
		(void)u32AHI_DioInterruptStatus(); // clear interrupt register
		vAHI_DioWakeEnable( u32DioPortWakeUp, 0); // ENABLE DIO WAKE SOURCE
		vAHI_DioWakeEdge(0, u32DioPortWakeUp ); // 割り込みエッジ(立下がりに設定)

		if( IS_APPCONF_OPT_ONESHOTMODE() && sAppData.bColdStart ){
			vSleep(10, FALSE, FALSE);
		}else if( bShortSleep ){
			vPortSetLo(WDT_OUT);
			// 割り込み待ちスリープに入る
			ToCoNet_vSleep(E_AHI_WAKE_TIMER_0, 60000, FALSE, FALSE);
		}else if (bContinuous){
			vSleep(60000, FALSE, FALSE);
		}else{
			if(!bContinuous && sAppData.u32SleepCount == 0 && sAppData.bWakeupByButton == TRUE){
				sAppData.u32SleepCount++;
			}

			// 周期スリープに入る
			vSleep(u32Sleep, sAppData.u16frame_count == 1 ? FALSE : TRUE, FALSE);
		}
		
	}
}

/**
 * イベント処理関数リスト
 */
static const tsToCoNet_Event_StateHandler asStateFuncTbl[] = {
	PRSEV_HANDLER_TBL_DEF(E_STATE_IDLE),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_BLINK_LED),
	PRSEV_HANDLER_TBL_DEF(E_STATE_RUNNING),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_JUDGE_SEND),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_WAIT_TX),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_SLEEP),
	PRSEV_HANDLER_TBL_TRM
};

/**
 * イベント処理関数
 * @param pEv
 * @param eEvent
 * @param u32evarg
 */
void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	ToCoNet_Event_StateExec(asStateFuncTbl, pEv, eEvent, u32evarg);
}

#if 0
/**
 * ハードウェア割り込み
 * @param u32DeviceId
 * @param u32ItemBitmap
 * @return
 */
static uint8 cbAppToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	uint8 u8handled = FALSE;

	switch (u32DeviceId) {
	case E_AHI_DEVICE_ANALOGUE:
		break;

	case E_AHI_DEVICE_SYSCTRL:
		break;

	case E_AHI_DEVICE_TIMER0:
		break;

	case E_AHI_DEVICE_TICK_TIMER:
		break;

	default:
		break;
	}

	return u8handled;
}
#endif

/**
 * ハードウェアイベント（遅延実行）
 * @param u32DeviceId
 * @param u32ItemBitmap
 */
static void cbAppToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	switch (u32DeviceId) {
	case E_AHI_DEVICE_TICK_TIMER:
		vProcessMC3630(E_EVENT_TICK_TIMER);
		break;

	case E_AHI_DEVICE_ANALOGUE:
		/*
		 * ADC完了割り込み
		 */
		V_PUTCHAR('@');
		vSnsObj_Process(&sAppData.sADC, E_ORDER_KICK);
		if (bSnsObj_isComplete(&sAppData.sADC)) {
			u8sns_cmplt |= E_SNS_ADC_CMP_MASK;
			vStoreSensorValue();
		}
		break;

	case E_AHI_DEVICE_SYSCTRL:
		break;

	case E_AHI_DEVICE_TIMER0:
		break;

	default:
		break;
	}
}

#if 0
/**
 * メイン処理
 */
static void cbAppToCoNet_vMain() {
	/* handle serial input */
	vHandleSerialInput();
}
#endif

#if 0
/**
 * ネットワークイベント
 * @param eEvent
 * @param u32arg
 */
static void cbAppToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	switch(eEvent) {
	case E_EVENT_TOCONET_NWK_START:
		break;

	default:
		break;
	}
}
#endif


#if 0
/**
 * RXイベント
 * @param pRx
 */
static void cbAppToCoNet_vRxEvent(tsRxDataApp *pRx) {

}
#endif

/**
 * TXイベント
 * @param u8CbId
 * @param bStatus
 */
static void cbAppToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	// 送信完了
	V_PRINTF(LB"! Tx Cmp = %d", bStatus);
	ToCoNet_Event_Process(E_ORDER_KICK, 0, vProcessEvCore);
}
/**
 * アプリケーションハンドラー定義
 *
 */
static tsCbHandler sCbHandler = {
	NULL, // cbAppToCoNet_u8HwInt,
	cbAppToCoNet_vHwEvent,
	NULL, // cbAppToCoNet_vMain,
	NULL, // cbAppToCoNet_vNwkEvent,
	NULL, // cbAppToCoNet_vRxEvent,
	cbAppToCoNet_vTxEvent
};

/**
 * アプリケーション初期化
 */
void vInitAppMOT() {
	psCbHandler = &sCbHandler;
	pvProcessEv = vProcessEvCore;
}

static void vProcessMC3630(teEvent eEvent) {
	if (bSnsObj_isComplete(&sSnsObj)) {
		return;
	}

	// イベントの処理
	vSnsObj_Process(&sSnsObj, eEvent); // ポーリングの時間待ち
	if (bSnsObj_isComplete(&sSnsObj)) {
		u8sns_cmplt |= E_SNS_MC3630_CMP;
		//u32EndTime = U64_LOWER_U32(u64GetTimer_ms());

		V_PRINTF( LB"!NUM = %d", sObjMC3630.u8FIFOSample );
		V_PRINTF(LB"!MC3630_%d: X : %d, Y : %d, Z : %d",
				0,
				sObjMC3630.ai16Result[MC3630_X][0],
				sObjMC3630.ai16Result[MC3630_Y][0],
				sObjMC3630.ai16Result[MC3630_Z][0]
		);

		// 完了時の処理
		if (u8sns_cmplt == E_SNS_ALL_CMP) {
			ToCoNet_Event_Process(E_ORDER_KICK, 0, vProcessEvCore);
		}
	}
}

/**
 * センサー値を格納する
 */
static void vStoreSensorValue() {
	// センサー値の保管
	sAppData.u16Adc[0] = sAppData.sObjADC.ai16Result[TEH_ADC_IDX_ADC_1];
	sAppData.u8Batt = ENCODE_VOLT(sAppData.sObjADC.ai16Result[TEH_ADC_IDX_VOLT]);
}

static void vCalc_Average_MinMax(int16 ai16accel[3][32], uint8 u8Sample)
{
	uint8 i;
	for( i=0; i<u8Sample; i++ ){
		uint8 j = 0;
		for(j=0; j<3; j++){
			ai32ave[j] += ai16accel[j][i];
			if( ai16accel[j][i] > ai16max[j] ) ai16max[j] = ai16accel[j][i];
			if( ai16accel[j][i] < ai16min[j] ) ai16min[j] = ai16accel[j][i];
		}
	}
	u16SamplingCount += u8Sample;
}

static bool_t bSendData( int16 ai16accel[3][32], uint8 u8startAddr, uint8 u8Sample )
{
	if(IS_APPCONF_OPT_2525AMODE()) return bSendAppTag( ai16accel, u8startAddr, u8Sample );

	uint8 i;
	uint8	au8Data[92];
	uint8*	q = au8Data;
	uint32 u32Time_ms = 0;
	uint16 u16freq = 0;

	uint8 datanum = 2;
	if(IS_APPCONF_OPT_AVERAGEMODE()) datanum += 5;
	else{
		datanum += u8Sample;
		if(IS_APPCONF_OPT_SEND_EXTRADATA()) datanum += 1;
	}

	if( sPALData.u8EEPROMStatus != 0 ){
		S_OCTET(datanum+1);		// データ数
		S_OCTET(0x32);
		S_OCTET(0x00);
		S_OCTET( sPALData.u8EEPROMStatus );
	}else{
		S_OCTET(datanum);		// データ数
	}

	S_OCTET(0x30);					// 電源電圧
	S_OCTET(0x01);
	S_OCTET(sAppData.u8Batt);

	S_OCTET(0x30);					// AI1(ADC1)
	S_OCTET(0x02);
	S_BE_WORD(sAppData.u16Adc[0]);

	if( IS_APPCONF_OPT_AVERAGEMODE() || IS_APPCONF_OPT_SEND_EXTRADATA() ){
		u32Time_ms  = u32EndTime-u32StartTime;
		u16freq = (((uint32)u16SamplingCount*10000)/u32Time_ms)&0xFFFF;
		V_PRINTF(LB"start = %d[ms]: end = %d[ms]", u32StartTime, u32EndTime);
		V_PRINTF(LB"time = %d[ms]: freq = %d[Hz]", u32Time_ms, u16freq);
	}

	if( IS_APPCONF_OPT_AVERAGEMODE() ){
		ai32ave[0] /= u16SamplingCount;
		ai32ave[1] /= u16SamplingCount;
		ai32ave[2] /= u16SamplingCount;

		S_OCTET(0x07);
		S_OCTET(0x01);
		S_BE_WORD(ai32ave[0]);
		S_BE_WORD(ai32ave[1]);
		S_BE_WORD(ai32ave[2]);

		S_OCTET(0x07);
		S_OCTET(0x02);
		S_BE_WORD(ai16min[0]);
		S_BE_WORD(ai16min[1]);
		S_BE_WORD(ai16min[2]);

		S_OCTET(0x07);
		S_OCTET(0x03);
		S_BE_WORD(ai16max[0]);
		S_BE_WORD(ai16max[1]);
		S_BE_WORD(ai16max[2]);

		S_OCTET(0x07);
		S_OCTET(0x81);
		S_BE_WORD(u16SamplingCount);

		S_OCTET(0x07);
		S_OCTET(0x82);
		S_BE_WORD(u16freq);

		u16SamplingCount = 0;
		uint8 i;
		for( i=0; i<3; i++ ){
			ai32ave[i] = 0;
			ai16max[i] = -20000;
			ai16min[i] = 20000; 
		}

	}else{
		if(u8Sample){
			S_OCTET(0x04);					// Acceleration
			S_OCTET( sObjMC3630.u8Interrupt );		// 拡張用1バイト
			S_OCTET( u8Sample );				// サンプル数
			S_OCTET( sObjMC3630.u8SampleFreq-MC3630_SAMPLING25HZ );		// サンプリング周波数
			S_OCTET( 12 );			// 送信するデータの分解能

			V_PRINTF( LB"sample = %d", u8Sample );
			V_PRINTF( LB"freq = %d", sObjMC3630.u8SampleFreq-MC3630_SAMPLING25HZ );

			uint16 u16x[2], u16y[2], u16z[2];
			for( i=u8startAddr; i<u8Sample+u8startAddr; i+=2 ){
				u16x[0] = (ai16accel[MC3630_X][i]>>3)&0x0FFF;
				u16x[1] = (ai16accel[MC3630_X][i+1]>>3)&0x0FFF;
				u16y[0] = (ai16accel[MC3630_Y][i]>>3)&0x0FFF;
				u16y[1] = (ai16accel[MC3630_Y][i+1]>>3)&0x0FFF;
				u16z[0] = (ai16accel[MC3630_Z][i]>>3)&0x0FFF;
				u16z[1] = (ai16accel[MC3630_Z][i+1]>>3)&0x0FFF;

				// 12bitに変換する
				S_OCTET((u16x[0]&0x0FF0)>>4);
				S_OCTET( ((u16x[0]&0x0F)<<4) | ((u16y[0]&0x0F00)>>8) );
				S_OCTET(u16y[0]&0x00FF);
				S_OCTET((u16z[0]&0x0FF0)>>4);
				S_OCTET( ((u16z[0]&0x0F)<<4) | ((u16x[1]&0x0F00)>>8) );
				S_OCTET(u16x[1]&0x00FF);
				S_OCTET((u16y[1]&0x0FF0)>>4);
				S_OCTET( ((u16y[1]&0x0F)<<4) | ((u16z[1]&0x0F00)>>8) );
				S_OCTET(u16z[1]&0x00FF);
			}
		}
		if( IS_APPCONF_OPT_SEND_EXTRADATA() ){
			S_OCTET(0x07);
			S_OCTET(0x82);
			S_BE_WORD(u16freq);
			u16SamplingCount = 0;
		}
	}

	sAppData.u16frame_count++;

	if( !bContinuous ){
		u8FIFOCount--;
	}

	return bTransmitToParent( sAppData.pContextNwk, au8Data, q-au8Data );
}

static bool_t bSendAppTag( int16 ai16accel[3][32], uint8 u8startAddr, uint8 u8Sample )
{
	uint8 i;
	uint8	au8Data[92];
	uint8*	q = au8Data;

	S_OCTET(sAppData.u8Batt);
	S_BE_WORD(sAppData.u16Adc[0]);
	S_BE_WORD(0x0000);

	if(IS_APPCONF_OPT_AVERAGEMODE()){
		ai32ave[0] /= u16SamplingCount;
		ai32ave[1] /= u16SamplingCount;
		ai32ave[2] /= u16SamplingCount;

		S_BE_WORD(ai16min[0]);
		S_BE_WORD(ai32ave[0]);
		S_BE_WORD(ai16max[0]);
		S_OCTET(0xF9);
		S_BE_WORD(u16SamplingCount);
		S_BE_WORD(ai16min[1]);
		S_BE_WORD(ai32ave[1]);
		S_BE_WORD(ai16max[1]);
		S_BE_WORD(ai16min[2]);
		S_BE_WORD(ai32ave[2]);
		S_BE_WORD(ai16max[2]);

		u16SamplingCount = 0;
		uint8 i;
		for( i=0; i<3; i++ ){
			ai32ave[i] = 0;
			ai16max[i] = -20000;
			ai16min[i] = 20000; 
		}

	}else{
		if(u8Sample == 1){
			S_BE_WORD(ai16accel[0][0]/10);
			S_BE_WORD(ai16accel[1][0]/10);
			S_BE_WORD(ai16accel[2][0]/10);
			S_OCTET(0xFE);
		}else{
			S_BE_WORD(ai16accel[0][0]);
			S_BE_WORD(ai16accel[1][0]);
			S_BE_WORD(ai16accel[2][0]);
			S_OCTET(0xFA);
			S_OCTET(u8Sample);

			for(i=1;i<u8Sample;i++){
				S_BE_WORD(ai16accel[0][i]);
				S_BE_WORD(ai16accel[1][i]);
				S_BE_WORD(ai16accel[2][i]);
			}
		}
	}
	sAppData.u16frame_count++;

	if( !bContinuous ){
		u8FIFOCount--;
	}

	return bTransmitToAppTag( sAppData.pContextNwk, au8Data, q-au8Data );
}