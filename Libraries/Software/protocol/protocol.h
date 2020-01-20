#pragma once

#include <stdint.h>

// Implement this function in your app
uint8_t PZ_crc(uint8_t const* data, uint8_t size);

#define PZ_VERSION_MAJOR ((uint8_t)0x1)
#define PZ_VERSION_NINOR ((uint8_t)0x0)

#define PZ_VERSION = ((PZ_VERSION_MAJOR << 4) | PZ_VERSION_NINOR)

#define PZ_FOOTPRINT ((uint8_t)0x55)
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

#pragma pack(push)
#pragma pack(1)
typedef struct PZ_Package {
	uint8_t fpr;
	uint8_t adr;
	uint8_t len;
	uint8_t rid;
	PZ_Cmd cmd;
	uint8_t pld[PZ_MAX_DATA_LEN];
	uint8_t crc;
} PZ_Package;
#pragma pack(pop)

uint8_t PZ_isResponse(PZ_Package const*);
uint8_t PZ_isAdrValid(uint8_t adr);
uint8_t PZ_pldLen(PZ_Cmd cmd);
PZ_Result PZ_verify(uint8_t const* package, uint8_t size);
PZ_Package PZ_fromData(uint8_t const* data);
void PZ_toData(uint8_t* outData, uint8_t* outDataSize, PZ_Package* package);

PZ_Package PZ_compose0(uint8_t adr, PZ_Cmd cmd);
PZ_Package PZ_compose1(uint8_t adr, PZ_Cmd cmd, uint8_t data0);
PZ_Package PZ_compose2(uint8_t adr, PZ_Cmd cmd, uint8_t data0, uint8_t data1);
PZ_Package PZ_compose(uint8_t adr, PZ_Cmd cmd, uint8_t const* data, uint8_t dataLen);
PZ_Package PZ_composeRe2(PZ_Package const * pck, uint8_t data0, uint8_t data1);
PZ_Package PZ_composeRe(PZ_Package const * pck, uint8_t const* data, uint8_t dataLen);

typedef int (*PZ_printf)(char const* character, ...);

void PZ_PrintInfo(PZ_printf, PZ_Package const*);

#define PZ_FEATHER_TYPE ((uint8_t)0x01)

typedef enum {
	PZ_Feather_CV_addres = 0x00, // The address of the device
	PZ_Feather_CV_motorDef = 0x10, // default value for DC motor on startup
	PZ_Feather_CV_motorCal1 = 0x11, // calibrating minimal value for DC motor
	PZ_Feather_CV_motorCal2 = 0x12, // calibrating maximal value for DC motor
	PZ_Feather_CV_motorCal0 = 0x13, // calibrating zero value for DC motor
	PZ_Feather_CV_servoDef = 0x20, // default value for the servo on startup
	PZ_Feather_CV_servoCal1 = 0x21, // calibrating minimal value for the servo
	PZ_Feather_CV_servoCal2 = 0x22, // calibrating maximal value for the servo
} PZ_Feather_CV;

typedef enum {
	PZ_Feather_DV_motor = 0x00, // value of DC motor
	PZ_Feather_DV_motorCalMode = 0x01, // DC motor calibrating mode
	PZ_Feather_DV_servo = 0x10, // value of servo
	PZ_Feather_DV_servoCalMode = 0x11, // servo calibrating mode
} PZ_Feather_DV;

#define PZ_KEEL_TYPE ((uint8_t)0x02)

typedef enum {
	PZ_Keel_CV_addres = 0x00, // The address of the device
} PZ_Keel_CV;

typedef enum {
	PZ_Keel_DV_bat0 = 0x00, // value of the battery0
	PZ_Keel_DV_bat1 = 0x01, // value of the battery1
	PZ_Keel_DV_thermo = 0x10, // value of the thermometer0
} PZ_Keel_DV;
