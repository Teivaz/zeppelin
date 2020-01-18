#pragma once

#include <stdint.h>

// Implement this function in your app
uint8_t PZ_crc(uint8_t* data, uint8_t size);

#define PZ_VERSION_MAJOR ((uint8_t)0x1)
#define PZ_VERSION_NINOR ((uint8_t)0x0)

#define PZ_VERSION = ((PZ_VERSION_MAJOR << 4) | PZ_VERSION_NINOR)

#define PZ_FOOTPRINT 0x55
#define PZ_MIN_PACKAGE_LEN 6
#define PZ_MAX_PACKAGE_LEN 8
#define PZ_MAX_DATA_LEN 2

typedef enum {
	PZ_Cmd_Info = 0x00,
	PZ_Cmd_Info_re = 0x80,
	PZ_Cmd_Read_cv = 0x01,
	PZ_Cmd_Read_cv_re = 0x81,
	PZ_Cmd_Write_cv = 0x02,
	PZ_Cmd_Reset_cv = 0x03,
	PZ_Cmd_Reset_all_cv = 0x04,
	PZ_Cmd_Read_dv = 0x05,
	PZ_Cmd_Read_dv_re = 0x85,
	PZ_Cmd_Write_dv = 0x06,
	PZ_Cmd_Reset_dv = 0x07,
	PZ_Cmd_Reset_all_dv = 0x08,
} PZ_Cmd;

typedef enum {
	PZ_Device_Feather = 0x01,
	PZ_Device_Keel = 0x02,
} PZ_Device;

typedef enum {
	PZ_OK = 0,
	PZ_ERROR = 1,
	PZ_ERROR_CRC,
} PZ_Result;

typedef struct PZ_Package {
	uint8_t fpr;
	uint8_t adr;
	uint8_t len;
	uint8_t rid;
	PZ_Cmd cmd;
	uint8_t pld[PZ_MAX_DATA_LEN];
	uint8_t crc;
} PZ_Package;

uint8_t PZ_isResponse(PZ_Package*);
uint8_t PZ_isAdrValid(uint8_t adr);
uint8_t PZ_pldLen(PZ_Cmd cmd);
PZ_Result PZ_verify(uint8_t* package, uint8_t size);
PZ_Package PZ_fromData(uint8_t* data);
void PZ_toData(uint8_t* outData, uint8_t* outDataSize, PZ_Package package);


typedef int (*PZ_printf)(const char* character, ...);

void PZ_PrintInfo(PZ_printf, PZ_Package*);

//void PZ_create(uint8_t* buf, PZ_Cmd cmd, uint8_t addr);
