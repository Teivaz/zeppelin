#pragma once

#include "hci_tl.h"

enum BleAppConStatus {
	EBleAppConStatus_Idle,
	EBleAppConStatus_FastAdv,
	EBleAppConStatus_BleLpAdv,
	EBleAppConStatus_BleScan,
	EBleAppConStatus_Connecting,
	EBleAppConStatus_ConnectedServer,
	EBleAppConStatus_ConnectedClient,

	EBleAppConStatus_DiscoverServices,
	EBleAppConStatus_DiscoverChars,
	EBleAppConStatus_DiscoverWriteDesc,
	EBleAppConStatus_DiscoverNotificationCharDesc,
	EBleAppConStatus_EnabledNotificationDesc,
	EBleAppConStatus_DisableNotificationDesc,
};

void BleAppInit(void); 

enum BleAppConStatus BleAppGetConnectionStatus(uint16_t connectionHandler);
