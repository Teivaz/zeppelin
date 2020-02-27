#pragma once

#include "constants.h"
#include <stdint.h>

char const* BtCompanyIdToString(uint16_t id);
char const* GattServiceToString(uint16_t id);

struct Uuid16 {
	uint8_t value[2];
};

uint16_t Uuid16ToInt(struct Uuid16 id);

struct Uuid32 {
	uint8_t value[4];
};

struct Uuid128 {
	uint8_t value[16];
};

struct BleDiscvocerDataFlags {
	uint8_t limitedDiscoverableMode :1;
	uint8_t generalDiscoverableMode :1;
	uint8_t brEdrNotSupported :1;
	uint8_t simultaneousLeAndBrEdrController :1;
	uint8_t simultaneousLeAndBrEdrHost :1;
	uint8_t reserved :3;
};

struct BleDiscoverData {
	struct BleDiscvocerDataFlags flags;
	int8_t txPower; // dBm
	enum DataTypeRole role;

	// The UTF-8 encoded name
	uint8_t const* name;
	uint8_t nameLength;
	uint8_t nameIsComplete;

	enum CharAppearance appearance;

	struct Uuid16* services16;
	uint8_t services16Count;
	uint8_t services16IsCompleteList;

	struct Uuid32* services32;
	uint8_t services32Count;
	uint8_t services32IsCompleteList;

	struct Uuid128* services128;
	uint8_t services128Count;
	uint8_t services128IsCompleteList;

	struct Uuid16 serviceData16;
	uint8_t const* serviceData16Additional;
	uint8_t serviceData16AdditionalLength;

	struct Uuid32 serviceData32;
	uint8_t const* serviceData32Additional;
	uint8_t serviceData32AdditionalLength;

	struct Uuid128 serviceData128;
	uint8_t const* serviceData128Additional;
	uint8_t serviceData128AdditionalLength;

	// https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers/
	enum BtCompanyId manufacturer;
	uint8_t const* manufacturerData;
	uint8_t manufacturerDataLength;
};

struct BleDiscoverData ReadBleDiscoverDataFromRaw(uint8_t const * advData, uint8_t advLength);
void pintBleDiscoverData(struct BleDiscoverData const*);

struct BleAddress {
	
};
