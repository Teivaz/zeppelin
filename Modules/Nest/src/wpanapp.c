#include "wpanapp.h"

#include "main.h"
#include "app_common.h"
#include "bleapp.h"
#include "ble.h"
#include "tl.h"
#include "stm32_seq.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"

#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH * 4U * DIVC((sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE), 4U))

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t sEventPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t sSystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t sSystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t sBleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

static void ConfigureSystemPower(void);
static void InitDebug(void);
static void InitTl(void);
static void SysStatusNot(SHCI_TL_CmdStatus_t status);
static void SysUserEventTx(void* pPayload);

void WpanAppInit(void) {
	ConfigureSystemPower();
	HW_TS_Init(hw_ts_InitMode_Full, getRtc());
	InitDebug();
	InitTl();
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == Bt1Pin) {
		UTIL_SEQ_SetTask(1 << EAppTask_ButtonPushed, 0);
	}
}

static void InitDebug(void) {
#if (CFG_DEBUGGER_SUPPORTED == 1)
	HAL_DBGMCU_EnableDBGSleepMode();

	LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);
	LL_C2_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);
#else

	GPIO_InitTypeDef gpio = {0};
	gpio.Pull = GPIO_NOPULL;
	gpio.Mode = GPIO_MODE_ANALOG;

	gpio.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpio);
	__HAL_RCC_GPIOA_CLK_DISABLE();

	gpio.Pin = GPIO_PIN_4 | GPIO_PIN_3;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &gpio);
	__HAL_RCC_GPIOB_CLK_DISABLE();

	HAL_DBGMCU_DisableDBGSleepMode();
	HAL_DBGMCU_DisableDBGStopMode();
	HAL_DBGMCU_DisableDBGStandbyMode();

#endif

#if (CFG_DEBUG_TRACE != 0)
	DbgTraceInit();
#endif
}

static void ConfigureSystemPower(void) {
	LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);
	UTIL_LPM_Init();
}

static void InitTl(void) {
	TL_Init();
	UTIL_SEQ_RegTask(1 << EAppTask_SystemHciAsyncEventId, UTIL_SEQ_RFU, shci_user_evt_proc);

	SHCI_TL_HciInitConf_t hciConf;
	hciConf.p_cmdbuffer = (uint8_t*)&sSystemCmdBuffer;
	hciConf.StatusNotCallBack = SysStatusNot;
	shci_init(SysUserEventTx, (void*)&hciConf);

	TL_MM_Config_t tlConfig;
	tlConfig.p_BleSpareEvtBuffer = sBleSpareEvtBuffer;
	tlConfig.p_SystemSpareEvtBuffer = sSystemSpareEvtBuffer;
	tlConfig.p_AsynchEvtPool = sEventPool;
	tlConfig.AsynchEvtPoolSize = POOL_SIZE;
	TL_MM_Init(&tlConfig);

	TL_Enable();
}

static void SysStatusNot(SHCI_TL_CmdStatus_t status) {
}

static void SysUserEventTx(void* payload) {
	TL_TRACES_Init();
	BleAppInit();
	UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
}

void UTIL_SEQ_Idle(void) {
#if (CFG_LPM_SUPPORTED == 1)
	UTIL_LPM_EnterLowPower();
#endif
}

void UTIL_SEQ_EvtIdle(UTIL_SEQ_bm_t task_id_bm, UTIL_SEQ_bm_t evt_waited_bm) {
	UTIL_SEQ_Run(UTIL_SEQ_DEFAULT);
}

void shci_notify_asynch_evt(void* data) {
	UTIL_SEQ_SetTask(1 << EAppTask_SystemHciAsyncEventId, 0);
}

void shci_cmd_resp_release(uint32_t flag) {
	UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
}

void shci_cmd_resp_wait(uint32_t timeout) {
	UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
}

#if (CFG_DEBUG_TRACE != 0)

// STM32_WPAN dbg_trace.h
void DbgOutputInit(void) {
}

// STM32_WPAN dbg_trace.h
void DbgOutputTraces(uint8_t* data, uint16_t size, void (*cb)(void)) {
	HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, data, size, cb);
}
#endif
