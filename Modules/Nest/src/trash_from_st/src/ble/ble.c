#include "structs.h"
#include <stdio.h>
#include <string.h>

uint16_t uint16FromArray(uint8_t const* data) {
	return ((uint16_t)data[1] << 8) | data[0];
}

uint16_t Uuid16ToInt(struct Uuid16 id) {
	return uint16FromArray(id.value);
}

struct BleDiscoverData ReadBleDiscoverDataFromRaw(uint8_t const* advData, uint8_t advLength) {
	struct BleDiscoverData result = {0};
	uint8_t const * const advDataEnd = advData + advLength;
	while (advData < advDataEnd) {
		uint8_t const length = advData[0];
		if (length == 0) {
			// According to the specs length 0 signals early end of package
			return result;
		}
		enum AdvData const dataType = (enum AdvData)advData[1];
		uint8_t const * const data = &advData[2];
		advData += length + 1;
		uint8_t dataLen = length - 1;

		switch (dataType) {
			case EAdvData_Flags:
				result.flags = *(struct BleDiscvocerDataFlags*)data;
				break;

			case EAdvData_TxPower:
				result.txPower = *(int8_t*)data;
				break;

			case EAdvData_LeRole:
				result.role = *(enum DataTypeRole*)data;
				break;

			case EAdvData_Services16Complete:
				result.services16IsCompleteList = 1;
				// fallthrough
			case EAdvData_Services16:
				result.services16 = (struct Uuid16*)data;
				result.services16Count = dataLen / 2;
				break;

			case EAdvData_Services32Complete:
				result.services32IsCompleteList = 1;
				// fallthrough
			case EAdvData_Services32:
				result.services32 = (struct Uuid32*)data;
				result.services32Count = dataLen / 4;
				break;

			case EAdvData_Services128Complete:
				result.services128IsCompleteList = 1;
				// fallthrough
			case EAdvData_Services128:
				result.services128 = (struct Uuid128*)data;
				result.services128Count = dataLen / 16;
				break;

			case EAdvData_NameComplete:
				result.nameIsComplete = 1;
				// fallthrough
			case EAdvData_Name:
				result.name = data;
				result.nameLength = dataLen;
				break;
	
			case EAdvData_Appearance:
				result.appearance = (enum CharAppearance)uint16FromArray(data);
				break;

			case EAdvData_ManufacturerSpecificData:
				result.manufacturer = (enum BtCompanyId)uint16FromArray(data);
				// The rest can be anything depending on device type and manufacturer
				if (dataLen > 2) {
					result.manufacturerData = data + 2;
					result.manufacturerDataLength = dataLen - 2;
				}
				break;

			case EAdvData_ServiceData:
				result.serviceData16 = *(struct Uuid16*)data;
				if (dataLen > 2) {
					result.serviceData16Additional = data + 2;
					result.serviceData16AdditionalLength = dataLen - 2;
				}
				break;

			case EAdvData_ServiceData32:
				result.serviceData32 = *(struct Uuid32*)data;
				if (dataLen > 4) {
					result.serviceData32Additional = data + 4;
					result.serviceData32AdditionalLength = dataLen - 4;
				}
				break;

			case EAdvData_ServiceData128:
				result.serviceData128 = *(struct Uuid128*)data;
				if (dataLen > 16) {
					result.serviceData128Additional = data + 16;
					result.serviceData128AdditionalLength = dataLen - 16;
				}
				break;

			default: break;
		}
	}
	return result;
}

void pintBleDiscoverData(struct BleDiscoverData const* d) {
	if (d->name) {
		char name[32] = {0};
		memcpy(name, d->name, d->nameLength);
		printf("Name: '%s'\r\n", name);
	}
	if (d->services16) {
		printf("Services UUID 16: %d\r\n", d->services16Count);
		for (uint8_t i = 0; i < d->services16Count; i++) {
			printf("%d - %s\r\n", i+1, GattServiceToString(Uuid16ToInt(d->services16[i])));
		}
	}
	char const * const dashes[] = {"", "", "", "-", "", "", "-", "", "-", "", "-", "", "", "", "", ""};
	if (d->services128Count > 0) {
		printf("Services UUID 128: %d\r\n", d->services128Count);
		for (uint8_t i = 16; i != 0; --i) {
			printf("%02X%s", d->services128[0].value[i-1], dashes[16-i]);
		}
		printf("\r\n");
	}
	if (d->serviceData16.value[0] != 0) {
		printf("Service data: %s (%04x)", GattServiceToString(Uuid16ToInt(d->serviceData16)), Uuid16ToInt(d->serviceData16));
		if (d->serviceData16AdditionalLength > 0) {
			printf("Additional data: ");
			for (uint8_t i = 0; i < d->serviceData16AdditionalLength; i++) {
				printf("%02X ", d->serviceData16Additional[i]);
			}
			printf("\r\n");
		}
	}
	if (d->manufacturer) {
		printf("Manufacturer: %s\r\n", BtCompanyIdToString(d->manufacturer));
		printf("Manufacturer data: ");
		for (uint8_t i = 0; i < d->manufacturerDataLength; i++) {
			printf("%02X ", d->manufacturerData[i]);
		}
		printf("\r\n");
	}
}
