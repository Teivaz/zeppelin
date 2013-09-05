#ifndef _DENDRITE_H_
#define _DENDRITE_H_

#include <stdint.h>
#include <avr/eeprom.h>

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
	uint8_t xRange;
	int8_t yCenter;
	uint8_t yRange;
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


inline TStickPosition Sum(TStickPosition a, TStickPosition b)
{
	TStickPosition c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

/**************************************
* Variables
**************************************/

extern TStickCalibration	s_stickCalibration[STICK_NUM];
extern TStickPosition		s_stickNext[STICK_NUM];
extern TStickPosition		s_stickCurrent[STICK_NUM];
extern TStickPosition		s_stickPreset[STICK_NUM];

extern TFeatherCalibration	s_featherCalibration[FEATHER_NUM];
extern TFeatherData			s_featherResult[FEATHER_NUM];

extern TEMode				s_dendriteMode;
extern TEStickState			s_dendriteState[STICK_NUM];

// EEPROM
extern uint8_t e_featherCalibration[sizeof(TFeatherCalibration) * FEATHER_NUM] EEMEM;
extern uint8_t e_stickCalibration[sizeof(TStickCalibration) * STICK_NUM] EEMEM;
extern uint8_t e_mode EEMEM;

/**************************************
* Functions
**************************************/

void DendriteInit();

void ReadStickRawData(TEStics num, uint8_t *x, uint8_t *y);

// Front-Back or Left-Right mode
void SwitchMode();

// Reads raw data, scales and stores to s_stickNext
void ReadStickData(TEStics num);
// Returns two symmetric floats basing on value and balance of stick
TStickPosition DecodeStickData(TEStics num);

void PresetStick(TEStics num);

// Write current feather value to preset
void ResetStick(TEStics num);

void SetFeatherData(TEFeathers num, float servo, float motor);
void UpdateDendrite();
char IsStickNearPrevious(TEStics num);
char IsStickNearZero(TEStics num);

void SetFeathers();

// Save all feather calibration values
void SaveFeatherCalibrationValues();
// Load all feather calibration values
void LoadCalibrationValues();

void SaveStickCalibrationValues();
void LoadStickCalibrationValues();

#endif //_DENDRITE_H_