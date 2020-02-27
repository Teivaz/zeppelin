#include "bleapp.h"
#include "app_common.h"
#include "ble_conf.h"
#include "shci.h"
#include "stm32_seq.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "ble.h"
#include "tl.h"
#include "otp.h"

//!
#include "p2p_client_app.h"

#include "ble/constants.h"
#include "ble/structs.h"

#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define BD_ADDR_SIZE_LOCAL 6

struct SecurityParams {
	uint8_t ioCapability;
	uint8_t mitmMode;
	uint8_t bondingMode;
	uint8_t oobDataPresent;
	uint8_t oobData[16];
	uint8_t useFixedPin;
	uint8_t encryptionKeySizeMin;
	uint8_t encryptionKeySizeMax;
	uint32_t fixedPin;
	uint8_t initiateSecurity;
};

struct ProfileGlobalContext {
	struct SecurityParams bleSecurityParam;
	uint16_t gapServiceHandle;
	uint16_t devNameCharHandle;
	uint16_t appearanceCharHandle;
  uint16_t connectionHandle;
	uint8_t advtServUuidLen;
	uint8_t advtServUuid[100];
};

struct ApplicationContext {
	struct ProfileGlobalContext BleApplicationContext_legacy;
	enum BleAppConnectionStatus connectionStatus;
	uint8_t switchOffGpioTimerId;
	uint8_t deviceServerFound;
};

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_CmdPacket_t sBleCmdBuffer;

static uint8_t const kBdAddr[BD_ADDR_SIZE_LOCAL] = {
	(uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000ff) >> 8*0),
	(uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000ff00) >> 8*1),
	(uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000ff0000) >> 8*2),
	(uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000ff000000) >> 8*3),
	(uint8_t)((CFG_ADV_BD_ADDRESS & 0x00ff00000000) >> 8*4),
	(uint8_t)((CFG_ADV_BD_ADDRESS & 0xff0000000000) >> 8*5),
};

static uint8_t sBdAddrUdn[BD_ADDR_SIZE_LOCAL];

static uint8_t const BLE_CFG_IR_VALUE[16] = CFG_BLE_IRK;
static uint8_t const BLE_CFG_ER_VALUE[16] = CFG_BLE_ERK;

static uint8_t sServerRemoteBdAddr[BD_ADDR_SIZE_LOCAL];
P2PC_APP_ConnHandle_Not_evt_t sNotificationHdl;

PLACE_IN_SECTION("BLE_APP_CONTEXT") static struct ApplicationContext sAppContext;

static void BleUserEvtRx(void* pPayload);
static void BleStatusNot(HCI_TL_CmdStatus_t status);
static void BleTlInit(void);
static void BleHciGapGattInit(void);
static uint8_t const* BleGetBdAddress(void);
static void ScanRequest(void);
static void ConnectRequest(void);

void BleAppInit(void) {
	SHCI_C2_Ble_Init_Cmd_Packet_t bleInitCmdPacket = {
		.Header = {0},
		.Param = {
			.pBleBufferAddress = 0, .BleBufferSize = 0,
			.NumAttrRecord = CFG_BLE_NUM_GATT_ATTRIBUTES,
			.NumAttrServ = CFG_BLE_NUM_GATT_SERVICES,
			.AttrValueArrSize = CFG_BLE_ATT_VALUE_ARRAY_SIZE,
			.NumOfLinks = CFG_BLE_NUM_LINK,
			.ExtendedPacketLengthEnable = CFG_BLE_DATA_LENGTH_EXTENSION,
			.PrWriteListSize = CFG_BLE_PREPARE_WRITE_LIST_SIZE,
			.MblockCount = CFG_BLE_MBLOCK_COUNT,
			.AttMtu = CFG_BLE_MAX_ATT_MTU,
			.SlaveSca = CFG_BLE_SLAVE_SCA,
			.MasterSca = CFG_BLE_MASTER_SCA,
			.LsSource = CFG_BLE_LSE_SOURCE,
			.MaxConnEventLength = CFG_BLE_MAX_CONN_EVENT_LENGTH,
			.HsStartupTime = CFG_BLE_HSE_STARTUP_TIME,
			.ViterbiEnable = CFG_BLE_VITERBI_MODE,
			.LlOnly = CFG_BLE_LL_ONLY,
			.HwVersion = 0,
		},
	};

	BleTlInit();
	UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);
	UTIL_SEQ_RegTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, hci_user_evt_proc);
	SHCI_C2_BLE_Init(&bleInitCmdPacket);
	BleHciGapGattInit();
	SVCCTL_Init();
	UTIL_SEQ_RegTask(1 << CFG_TASK_START_SCAN_ID, UTIL_SEQ_RFU, ScanRequest);
	UTIL_SEQ_RegTask(1 << CFG_TASK_CONN_DEV_1_ID, UTIL_SEQ_RFU, ConnectRequest);

	sAppContext.connectionStatus = APP_BLE_IDLE;

	//!
	P2PC_APP_Init();
	
	UTIL_SEQ_SetTask(1 << CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
}

enum BleAppConnectionStatus BleAppGetConnectionStatus(uint16_t connectionHandle) {
  if (sAppContext.BleApplicationContext_legacy.connectionHandle == connectionHandle) {
    return sAppContext.connectionStatus;
  }
  return APP_BLE_IDLE;
}

static void BleTlInit(void) {
	HCI_TL_HciInitConf_t conf = {
		.p_cmdbuffer = (uint8_t*)&sBleCmdBuffer,
		.StatusNotCallBack = BleStatusNot,
	};
	hci_init(BleUserEvtRx, (void*)&conf);
}

static void BleHciGapGattInit(void){
	hci_reset();

	uint8_t const* bdAddr = BleGetBdAddress();

	aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, (uint8_t*)bdAddr);

	uint32_t staticRandomAddr[2];
	staticRandomAddr[1] = 0x0000ED6E;
	staticRandomAddr[0] = LL_FLASH_GetUDN();
	aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)staticRandomAddr);

	aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, (uint8_t*)BLE_CFG_IR_VALUE);
	aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, (uint8_t*)BLE_CFG_ER_VALUE);

	aci_hal_set_tx_power_level(1, CFG_TX_POWER);
	aci_gatt_init();

	uint8_t role = 0;
#if (BLE_CFG_PERIPHERAL == 1)
	role |= GAP_PERIPHERAL_ROLE;
#endif

#if (BLE_CFG_CENTRAL == 1)
	role |= GAP_CENTRAL_ROLE;
#endif
	uint16_t appearance[1] = { BLE_CFG_GAP_APPEARANCE }; 

	uint16_t gapServiceHandle, gapDevNameCharHandle, gapAppearanceHandle;
	if (role > 0) {
		char const* name = "Sasha";
		aci_gap_init(role, 0, APPBLE_GAP_DEVICE_NAME_LENGTH, &gapServiceHandle, &gapDevNameCharHandle, &gapAppearanceHandle);
		if (aci_gatt_update_char_value(gapServiceHandle, gapDevNameCharHandle, 0, strlen(name), (uint8_t*)name)) {
			BLE_DBG_SVCCTL_MSG("Device Name aci_gatt_update_char_value failed.\r\n");
		}
	}

	if(aci_gatt_update_char_value(gapServiceHandle, gapAppearanceHandle, 0, 2, (uint8_t *)&appearance)) {
		BLE_DBG_SVCCTL_MSG("Appearance aci_gatt_update_char_value failed.\r\n");
	}

	sAppContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability = CFG_IO_CAPABILITY;
	aci_gap_set_io_capability(sAppContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability);

	sAppContext.BleApplicationContext_legacy.bleSecurityParam.mitmMode = CFG_MITM_PROTECTION;
	sAppContext.BleApplicationContext_legacy.bleSecurityParam.oobDataPresent = 0;
	sAppContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = 8;
	sAppContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = 16;
	sAppContext.BleApplicationContext_legacy.bleSecurityParam.useFixedPin = 1;
	sAppContext.BleApplicationContext_legacy.bleSecurityParam.fixedPin = 111111;
	sAppContext.BleApplicationContext_legacy.bleSecurityParam.bondingMode = 1;

	for (uint8_t i = 0; i < 16; ++i) {
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.oobData[i] = i;
	}

	aci_gap_set_authentication_requirement(
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.bondingMode,
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.mitmMode,
		1,
		0,
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.useFixedPin,
		sAppContext.BleApplicationContext_legacy.bleSecurityParam.fixedPin,
		0
	);

	if (sAppContext.BleApplicationContext_legacy.bleSecurityParam.bondingMode) {
		aci_gap_configure_whitelist();
	}
}

static void ScanRequest(void) {
	if (sAppContext.connectionStatus != APP_BLE_CONNECTED_CLIENT) {
		tBleStatus const result = aci_gap_start_general_discovery_proc(SCAN_P, SCAN_L, PUBLIC_ADDR, 1);
		if (result == BLE_STATUS_SUCCESS) {
			APP_DBG_MSG("\r\n** START GENERAL DISCOVERY (SCAN) **\r\n");
		}
		else {
			APP_DBG_MSG("-- aci_gap_start_general_discovery_proc, Failed\r\n");
		}
	}
}

static void ConnectRequest(void) {
	APP_DBG_MSG("\r\n** CREATE CONNECTION TO SERVER **\r\n");
	if (sAppContext.connectionStatus != APP_BLE_CONNECTED_CLIENT) {
		tBleStatus const result = aci_gap_create_connection(
			SCAN_P, SCAN_L,
			PUBLIC_ADDR, sServerRemoteBdAddr,
			PUBLIC_ADDR,
			CONN_P1, CONN_P2,
			0,
			SUPERV_TIMEOUT,
			CONN_L1, CONN_L2
		);

		if (result == BLE_STATUS_SUCCESS) {
			sAppContext.connectionStatus = APP_BLE_LP_CONNECTING;
		}
		else {
			sAppContext.connectionStatus = APP_BLE_IDLE;
		}
	}
}

static void SwitchOffGpio(void){
}

uint8_t const* BleGetBdAddress(void) {
	uint8_t const* bdAddr;
	uint32_t udn = LL_FLASH_GetUDN();

	if (udn != 0xFFFFFFFF) {
		uint32_t companyId = LL_FLASH_GetSTCompanyID();
		uint32_t deviceId = LL_FLASH_GetDeviceID();

		sBdAddrUdn[0] = (uint8_t)((udn & 0x000000FF) >> 0);
		sBdAddrUdn[1] = (uint8_t)((udn & 0x0000FF00) >> 8);
		sBdAddrUdn[2] = (uint8_t)((udn & 0x00FF0000) >> 16);
		sBdAddrUdn[3] = (uint8_t)deviceId;
		sBdAddrUdn[4] = (uint8_t)((companyId & 0x000000FF) >> 0);
		sBdAddrUdn[5] = (uint8_t)((companyId & 0x0000FF00) >> 8);

		bdAddr = (uint8_t const*)sBdAddrUdn;
	}
	else {
		uint8_t* otpAddr = OTP_Read(0);
		if (otpAddr) {
			bdAddr = ((OTP_ID0_t*)otpAddr)->bd_address;
		}
		else {
			bdAddr = kBdAddr;
		}
	}
	return bdAddr;
}

void hci_notify_asynch_evt(void* pdata) {
	UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
}

void hci_cmd_resp_release(uint32_t flag) {
	UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
}

void hci_cmd_resp_wait(uint32_t timeout) {
	UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
}

static void BleUserEvtRx(void* payload) {
	tHCI_UserEvtRxParam* param = (tHCI_UserEvtRxParam*)payload; 
	
	SVCCTL_UserEvtFlowStatus_t const status = SVCCTL_UserEvtRx((void*)&(param->pckt->evtserial));
	if (status != SVCCTL_UserEvtFlowDisable) {
		param->status = HCI_TL_UserEventFlow_Enable;
	}
	else {
		param->status = HCI_TL_UserEventFlow_Disable;
	}
}

static void BleStatusNot(HCI_TL_CmdStatus_t status) {
	uint32_t taskIdList;
	switch (status) {
		case HCI_TL_CmdBusy:
			/**
			 * All tasks that may send an aci/hci commands shall be listed here
			 * This is to prevent a new command being sent while one is already pending
			 */
			taskIdList = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
			UTIL_SEQ_PauseTask(taskIdList);
			break;

		case HCI_TL_CmdAvailable:
			/**
			 * All tasks that may send an aci/hci commands shall be listed here
			 * This is to prevent a new command being sent while one is already pending
			 */
			taskIdList = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
			UTIL_SEQ_ResumeTask(taskIdList);
			break;

		default:
			break;
	}
}

void SVCCTL_ResumeUserEventFlow(void) {
	hci_resume_flow();
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void* pckt) {
	hci_event_pckt *event_pckt;
	evt_le_meta_event *meta_evt;
	hci_le_connection_complete_event_rp0 * connection_complete_event;
	evt_blue_aci *blue_evt;
	hci_le_advertising_report_event_rp0 * le_advertising_event;
	event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
	hci_disconnection_complete_event_rp0 *cc = (void *) event_pckt->data;
	uint8_t result;
	uint8_t event_type, event_data_size;
	int k = 0;
	uint8_t adtype, adlength;

	switch (event_pckt->evt) {
		case EVT_VENDOR: {
			sNotificationHdl.P2P_Evt_Opcode = PEER_DISCON_HANDLE_EVT;
			blue_evt = (evt_blue_aci*) event_pckt->data;
			switch (blue_evt->ecode) {
				case EVT_BLUE_GAP_PROCEDURE_COMPLETE: {
					aci_gap_proc_complete_event_rp0 *gap_evt_proc_complete = (void*) blue_evt->data;
					/* CHECK GAP GENERAL DISCOVERY PROCEDURE COMPLETED & SUCCEED */
					if (gap_evt_proc_complete->Procedure_Code == GAP_GENERAL_DISCOVERY_PROC && gap_evt_proc_complete->Status == 0x00) {
						APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE_COMPLETED\r\n");
						/*if a device found, connect to it, device 1 being chosen first if both found*/
						if (sAppContext.deviceServerFound == 0x01 && sAppContext.connectionStatus != APP_BLE_CONNECTED_CLIENT) {
							UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_DEV_1_ID, CFG_SCH_PRIO_0);
						}
					}
					break;
				}
				default:
					break;
			
			}
		}
		break; 

		case EVT_DISCONN_COMPLETE: {
			if (cc->Connection_Handle == sAppContext.BleApplicationContext_legacy.connectionHandle) {
				sAppContext.BleApplicationContext_legacy.connectionHandle = 0;
				sAppContext.connectionStatus = APP_BLE_IDLE;
				APP_DBG_MSG("\r\n\r** DISCONNECTION EVENT WITH SERVER \n");
				sNotificationHdl.P2P_Evt_Opcode = PEER_DISCON_HANDLE_EVT;
				sNotificationHdl.ConnectionHandle = sAppContext.BleApplicationContext_legacy.connectionHandle;
				P2PC_APP_Notification(&sNotificationHdl);
			}
		}
		break;

		case EVT_LE_META_EVENT: {
			meta_evt = (evt_le_meta_event*) event_pckt->data;
			switch (meta_evt->subevent) {
				case EVT_LE_CONN_COMPLETE: {
					/**
					 * The connection is done, 
					 */
					connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;
					sAppContext.BleApplicationContext_legacy.connectionHandle = connection_complete_event->Connection_Handle;
					sAppContext.connectionStatus = APP_BLE_CONNECTED_CLIENT;

					/* CONNECTION WITH CLIENT */
					APP_DBG_MSG("\r\n\r** CONNECTION EVENT WITH SERVER \n");
					sNotificationHdl.P2P_Evt_Opcode = PEER_CONN_HANDLE_EVT;
					sNotificationHdl.ConnectionHandle = sAppContext.BleApplicationContext_legacy.connectionHandle;
					P2PC_APP_Notification(&sNotificationHdl);

					result = aci_gatt_disc_all_primary_services(sAppContext.BleApplicationContext_legacy.connectionHandle);
					if (result == BLE_STATUS_SUCCESS) {
						APP_DBG_MSG("\r\n\r** GATT SERVICES & CHARACTERISTICS DISCOVERY  \n");
						APP_DBG_MSG("* GATT :  Start Searching Primary Services \r\n\r");
					}
					else {
						APP_DBG_MSG("BLE_CTRL_App_Notification(), All services discovery Failed \r\n\r");
					}
					break; /* HCI_EVT_LE_CONN_COMPLETE */
				}

				case EVT_LE_ADVERTISING_REPORT: {
					uint8_t *adv_report_data;
					le_advertising_event = (hci_le_advertising_report_event_rp0 *) meta_evt->data;
					event_type = le_advertising_event->Advertising_Report[0].Event_Type;
					event_data_size = le_advertising_event->Advertising_Report[0].Length_Data;
					adv_report_data = (uint8_t*)(&le_advertising_event->Advertising_Report[0].Length_Data) + 1;
					k = 0;

					/* search AD TYPE 0x09 (Complete Local Name) */
					/* search AD Type 0x02 (16 bits UUIDS) */
					if (event_type == ADV_IND || event_type == ADV_DIRECT_IND || event_type == ADV_SCAN_IND || event_type == ADV_NONCONN_IND || event_type == SCAN_RSP) {
						uint8_t const* addr = le_advertising_event->Advertising_Report[0].Address;
						int8_t const rssi = *(int8_t*) (adv_report_data + le_advertising_event->Advertising_Report[0].Length_Data);
						printf("\n\r-- %02X:%02X:%02X:%02X:%02X:%02X (%ddB) --\r\n", addr[5],addr[4],addr[3],addr[2],addr[1],addr[0], rssi);
						switch (event_type) {
							case 0: printf("ADV_IND"); break;
							case 1: printf("ADV_DIRECT_IND"); break;
							case 2: printf("ADV_NONCONN_IND"); break;
							case 3: printf("SCAN_REQ"); break;
							case 4: printf("SCAN_RSP"); break;
						}
						printf("\r\n");
						struct BleDiscoverData d = ReadBleDiscoverDataFromRaw(adv_report_data, event_data_size);
						pintBleDiscoverData(&d);
						printf("--------- Package End ---------\r\n");

						/* ISOLATION OF BD ADDRESS AND LOCAL NAME */

						while(k < event_data_size) {
							adlength = adv_report_data[k];
							adtype = adv_report_data[k + 1];
							switch (adtype) {
								case AD_TYPE_FLAGS: /* now get flags */
								break;

								case AD_TYPE_TX_POWER_LEVEL: /* Tx power level */
								break;

								case AD_TYPE_MANUFACTURER_SPECIFIC_DATA: /* Manufacturer Specific */
									if (adlength >= 7 && adv_report_data[k + 2] == 0x01) { /* ST VERSION ID 01 */
										APP_DBG_MSG("--- ST MANUFACTURER ID --- \n");
										switch (adv_report_data[k + 3]) {   /* Demo ID */
											case CFG_DEV_ID_P2P_SERVER1: /* End Device 1 */
												APP_DBG_MSG("-- SERVER DETECTED -- VIA MAN ID\n");
												sAppContext.deviceServerFound = 0x01;
												sServerRemoteBdAddr[0] = le_advertising_event->Advertising_Report[0].Address[0];
												sServerRemoteBdAddr[1] = le_advertising_event->Advertising_Report[0].Address[1];
												sServerRemoteBdAddr[2] = le_advertising_event->Advertising_Report[0].Address[2];
												sServerRemoteBdAddr[3] = le_advertising_event->Advertising_Report[0].Address[3];
												sServerRemoteBdAddr[4] = le_advertising_event->Advertising_Report[0].Address[4];
												sServerRemoteBdAddr[5] = le_advertising_event->Advertising_Report[0].Address[5];
												break;

											default:
												break;
										}
										break;
									}
								case AD_TYPE_SERVICE_DATA: /* service data 16 bits */
									break;

								default:
									break;
							} /* end switch Data[k+adlength] */
							k += adlength + 1;
						}
					}
				}
				break;

				default:
				break;
			}
		}
		break; /* HCI_EVT_LE_META_EVENT */

		default:
		break;
	}
	return SVCCTL_UserEvtFlowEnable;
}
