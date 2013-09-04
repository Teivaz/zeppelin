#ifndef _DENDRITE_H_
#define _DENDRITE_H_
#include <stdint.h>
/**************************************
* This module collects data from controls and converts it
**************************************/

/**************************************
* Constants
**************************************/

#define THRESHOLD 0.05f

/**************************************
* Enumerators
**************************************/

typedef enum
{
	EFrontBack = 0,
	ELeftRight
} TEMode;

typedef enum
{
	EFrontLeftFeather = 0,
	EFrontRightFeather,
	EBackLeftFeather,		
	EBackRightFeather,		
	
	FEATHER_NUM
} TEFeathers;

typedef enum
{
	EFrontLeftServo = 0,
	EFrontLeftMotor,
	EBackRightServo,
	EBackRightMotor,
	
	STICK_NUM
} TEStics;

typedef enum
{
	EStickIdle = 0,		// Basic Working state
	EStickShouldSet,	// Stick button is pressed so we should save position to preset
	EStickThresheld,	// Stick button is within threshold and should not change coordinates
	EStick
	
} TEStickState;

/**************************************
* Data types
**************************************/

typedef struct
{
	float x;		// Balance
	float y;		// Value
} TStickPosition;

typedef struct 
{
	int8_t xCenter;
	int8_t xRange;
	int8_t yCenter;
	int8_t yRange;
} TStickCalibration;

typedef struct
{
	uint8_t servo;
	int8_t	motor;
} TFeatherData;

typedef struct
{
	float a;
	float b;
} TVector2f;

typedef struct
{
	uint8_t servoA;	// Servo max down position
	uint8_t servoB; // Servo max up position
	uint8_t servoC; // Servo center position
	
	int8_t motorA;	// Motor max forward spin speed
	int8_t motorB;	// Motor max backward spin speed
	int8_t motorC;	// Motor stop
} TFeatherCalibration;

/**************************************
* Variables
**************************************/

extern TStickCalibration	s_stickCalibration[STICK_NUM];
extern TStickPosition		s_stickCurrent[STICK_NUM];
extern TStickPosition		s_stickPrevious[STICK_NUM];

extern TFeatherCalibration	s_FeatherCalibration[FEATHER_NUM];
extern TFeatherData			s_FeatherPreset[FEATHER_NUM];
extern TFeatherData			s_FeatherResult[FEATHER_NUM];

extern TEMode				s_DendriteMode;
extern TEStickState			s_DendriteState[STICK_NUM];

/**************************************
* Functions
**************************************/

void ReadStickRawData(TEStics num, uint8_t *x, uint8_t *y);

// Front-Back or Left-Right mode
void SwitchMode();

// Reads raw data, scales and stores to s_stickCurrent
void ReadStickData(TEStics num);
// Returns two symmetric floats basing on value and balance of stick
TVector2f DecodeStickData(TEStics num);

// Write current feather value to preset
void PresetFeather(TEFeathers num);
// Write current feather value to preset
void ResetFeather(TEFeathers num);

void SetFeatherData(TEFeathers num, float servo, float motor);
void UpdateDendrite();
char IsStickNearPrevious(TEStics num);
char IsStickNearZero(TEStics num);

void SetFeathers();


// Save all feather calibration values
void SaveCalibrationValues(TFeatherCalibration values[FEATHER_NUM]);
// Load all feather calibration values
void LoadCalibrationValues(TFeatherCalibration values[FEATHER_NUM]);


#endif //_DENDRITE_H_