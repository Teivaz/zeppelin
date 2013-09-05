#include "Dendrite.h"
#include <avr/eeprom.h>

uint8_t e_featherCalibration[sizeof(TFeatherCalibration) * FEATHER_NUM] =\
{
	0,		128,	255,
	0,		128,	255,
	0,		128,	255,
	0,		128,	255,
	-128,	0,		127,
	-128,	0,		127,
	-128,	0,		127,
	-128,	0,		127
};

uint8_t e_stickCalibration[sizeof(TStickCalibration) * STICK_NUM] = \
{
	0,	255,
	0,	255,
	0,	255,
	0,	255
};

uint8_t e_mode = EFrontBack;

void DendriteInit()
{
	s_dendriteMode = eeprom_read_byte(&e_mode);	
}


void SaveFeatherCalibrationValues()
{
	/*uint8_t* data = (uint8_t*)values;
	for(uint8_t a = 0; a < (sizeof(TFeatherCalibration) * FEATHER_NUM); ++a)
	{
		eeprom_write_byte(e_featherCalibration + a, data[a]);
	}*/
	eeprom_write_block(s_featherCalibration, e_featherCalibration, sizeof(TFeatherCalibration) * FEATHER_NUM);
}

void LoadCalibrationValues()
{
	/*uint8_t* data = (uint8_t*)values;
	for(uint8_t a = 0; a < (sizeof(TFeatherCalibration) * FEATHER_NUM); ++a)
	{
		data[a] = eeprom_read_byte(e_featherCalibration + a);
	}*/
	eeprom_read_block(e_featherCalibration, s_featherCalibration, sizeof(TFeatherCalibration) * FEATHER_NUM);
}

void SwitchMode()
{
	s_dendriteMode = (s_dendriteMode == EFrontBack) ? ELeftRight : EFrontBack;
	for(uint8_t a = 0; a < STICK_NUM; ++a)
	{
		ResetStick(a);
	}
	eeprom_write_byte(&e_mode, s_dendriteMode);
}

void ReadStickData(TEStics num)
{
	uint8_t stickX, stickY;
	ReadStickRawData(num, &stickX, &stickY);
	
	s_stickNext[num].x = (stickX - s_stickCalibration[num].xCenter)/s_stickCalibration[num].xRange;
	s_stickNext[num].y = (stickY - s_stickCalibration[num].yCenter)/s_stickCalibration[num].yRange;
}

TStickPosition DecodeStickData(TEStics num)
{
	TStickPosition decoded;
	decoded.x = s_stickCurrent[num].y - s_stickCurrent[num].x;
	decoded.y = s_stickCurrent[num].y + s_stickCurrent[num].x;
	if(abs(decoded.x) > 1.0f)
	{
		decoded.x = (decoded.x > 0) ? 1.0f : -1.0f;
	}
	if(abs(decoded.y) > 1.0f)
	{
		decoded.y = (decoded.y > 0) ? 1.0f : -1.0f;
	}
	return decoded;
}

void PresetStick(TEStics num)
{
	s_stickPreset[num].x = s_stickCurrent[num].x;
	s_stickPreset[num].y = s_stickCurrent[num].y;
}

void ResetStick(TEStics num)
{
	s_stickPreset[num].x = 0;
	s_stickPreset[num].y = 0;
}

void SetFeatherData(TEFeathers num, float servo, float motor)
{
	if(servo > 0)
	{
		s_featherResult[num].servo = servo * (s_featherCalibration[num].servoB - s_featherCalibration[num].servoC);
		s_featherResult[num].servo += s_featherCalibration[num].servoC;
	}
	else if(servo < 0)
	{
		s_featherResult[num].servo = s_featherCalibration[num].servoC;
		s_featherResult[num].servo = -servo * (s_featherCalibration[num].servoC - s_featherCalibration[num].servoA);
	}
	else
	{
		s_featherResult[num].servo = 0;
	}
	
	if(motor > 0)
	{
		s_featherResult[num].motor = motor * (s_featherCalibration[num].motorB - s_featherCalibration[num].motorC);
		s_featherResult[num].motor += s_featherCalibration[num].motorC;
	}
	else if(motor < 0)
	{
		s_featherResult[num].motor = s_featherCalibration[num].motorC;
		s_featherResult[num].motor = -motor * (s_featherCalibration[num].motorC - s_featherCalibration[num].motorA);
	}
	else
	{
		s_featherResult[num].motor = 0;
	}
}

void SetFeathers()
{
	switch(s_dendriteMode)
	{
		case EFrontBack:
		{
			TStickPosition servosFront = Sum(DecodeStickData(EFrontLeftServo), s_stickPreset[EFrontLeftServo]);
			TStickPosition motorsFront = Sum(DecodeStickData(EFrontLeftMotor), s_stickPreset[EFrontLeftMotor]);
			TStickPosition servosBack  =	Sum(DecodeStickData(EBackRightServo), s_stickPreset[EBackRightServo]);
			TStickPosition motorsBack  =	Sum(DecodeStickData(EBackRightMotor), s_stickPreset[EBackRightMotor]);
			SetFeatherData(EFrontLeftFeather,	servosFront.x,	motorsFront.x);
			SetFeatherData(EFrontRightFeather,	servosFront.y,	motorsFront.y);
			SetFeatherData(EBackLeftFeather,	servosBack.x,	motorsBack.x);
			SetFeatherData(EBackRightFeather,	servosBack.y,	motorsBack.y);
		}
		break;
		
		case ELeftRight:
		{
			TStickPosition servosLeft  = Sum(DecodeStickData(EFrontLeftServo), s_stickPreset[EFrontLeftServo]);
			TStickPosition motorsLeft  = Sum(DecodeStickData(EFrontLeftMotor), s_stickPreset[EFrontLeftMotor]);
			TStickPosition servosRight = Sum(DecodeStickData(EBackRightServo), s_stickPreset[EBackRightServo]);
			TStickPosition motorsRight = Sum(DecodeStickData(EBackRightMotor), s_stickPreset[EBackRightMotor]);
			SetFeatherData(EFrontLeftFeather,	servosLeft.x,	motorsLeft.x);
			SetFeatherData(EFrontRightFeather,	servosRight.x,	motorsRight.x);
			SetFeatherData(EBackLeftFeather,	servosLeft.y,	motorsLeft.y);
			SetFeatherData(EBackRightFeather,	servosRight.y,	motorsRight.y);
		}
		break;
	}
}

char IsStickNearPrevious(TEStics num)
{
	float max = s_stickCurrent[num].x + THRESHOLD;
	float min = s_stickCurrent[num].x - THRESHOLD;
	if(	(s_stickNext[num].x < max) && (s_stickNext[num].x > min) )
	{
		return 1;
	}
	max = s_stickCurrent[num].y + THRESHOLD;
	min = s_stickCurrent[num].y - THRESHOLD;
	if(	(s_stickNext[num].y < max) && (s_stickNext[num].y > min) )
	{
		return 1;
	}
	return 0;
}

char IsStickNearZero(TEStics num)
{
	if( (s_stickCurrent[num].x < THRESHOLD) && (s_stickCurrent[num].x > -THRESHOLD) )
	{
		return 1;
	}
	if( (s_stickCurrent[num].y < THRESHOLD) && (s_stickCurrent[num].y > -THRESHOLD) )
	{
		return 1;
	}
	return 0;
}

void UpdateDendrite()
{
	for(uint8_t n = 0; n < STICK_NUM; ++n)
	{
		switch(s_dendriteState[n])
		{
			case EStickIdle:
			{
				s_stickCurrent[n] = s_stickNext[n];
			}
			case EStickThresheld:
			{
				if(0 == IsStickNearPrevious(n))
				{
					s_stickCurrent[n] = s_stickNext[n];
					s_dendriteState[n] = EStickIdle;
				}
			}
			break;
			case EStickShouldSet:
			{
				if(IsStickNearZero(n))
				{
					ResetStick(n);
				}
				else
				{
					PresetStick(n);
					// Next state will not change data while stick is near
					s_dendriteState[n] = EStickThresheld;
				}
			}
			break;
		}
	}
	
	SetFeathers();
}

void SaveStickCalibrationValues()
{
	eeprom_write_block(s_stickCalibration, e_stickCalibration, sizeof(TStickCalibration) * STICK_NUM);
}

void LoadStickCalibrationValues()
{
	eeprom_read_block(e_stickCalibration, s_stickCalibration, sizeof(TStickCalibration) * STICK_NUM);
}
