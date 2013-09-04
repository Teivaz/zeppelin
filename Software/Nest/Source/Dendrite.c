#include "Dendrite.h"

void SwitchMode()
{
	s_DendriteMode = (s_DendriteMode == EFrontBack) ? ELeftRight : EFrontBack;
}

void ReadStickData(TEStics num)
{
	uint8_t stickX, stickY;
	ReadStickRawData(num, &stickX, &stickY);
	
	s_stickCurrent[num].x = (stickX - s_stickCalibration[num].xCenter)/s_stickCalibration[num].xRange;
	s_stickCurrent[num].y = (stickY - s_stickCalibration[num].yCenter)/s_stickCalibration[num].yRange;
}

TVector2f DecodeStickData(TEStics num)
{
	TVector2f decoded;
	decoded.a = s_stickPrevious[num].y - s_stickPrevious[num].x;
	decoded.b = s_stickPrevious[num].y + s_stickCurrent[num].x;
	if(abs(decoded.a) > 1.0f)
	{
		decoded.a = (decoded.a > 0) ? 1.0f : -1.0f;
	}
	if(abs(decoded.b) > 1.0f)
	{
		decoded.b = (decoded.b > 0) ? 1.0f : -1.0f;
	}
	return decoded;
}

void PresetFeather(TEFeathers num)
{
	s_FeatherPreset[num].servo = s_FeatherResult[num].servo;
	s_FeatherPreset[num]. motor = s_FeatherResult[num].motor;
}

void ResetFeather(TEFeathers num)
{
	s_FeatherPreset[num].servo = 0;
	s_FeatherPreset[num].motor = 0;
}

void SetFeatherData(TEFeathers num, float servo, float motor)
{
	if(servo > 0)
	{
		s_FeatherResult[num].servo = servo * (s_FeatherCalibration[num].servoB - s_FeatherCalibration[num].servoC);
		s_FeatherResult[num].servo += s_FeatherCalibration[num].servoC;
	}
	else if(servo < 0)
	{
		s_FeatherResult[num].servo = s_FeatherCalibration[num].servoC;
		s_FeatherResult[num].servo = -servo * (s_FeatherCalibration[num].servoC - s_FeatherCalibration[num].servoA);
	}
	else
	{
		s_FeatherResult[num].servo = 0;
	}
	s_FeatherResult[num].servo += s_FeatherPreset[num].servo;
	
	if(motor > 0)
	{
		s_FeatherResult[num].motor = motor * (s_FeatherCalibration[num].motorB - s_FeatherCalibration[num].motorC);
		s_FeatherResult[num].motor += s_FeatherCalibration[num].motorC;
	}
	else if(motor < 0)
	{
		s_FeatherResult[num].motor = s_FeatherCalibration[num].motorC;
		s_FeatherResult[num].motor = -motor * (s_FeatherCalibration[num].motorC - s_FeatherCalibration[num].motorA);
	}
	else
	{
		s_FeatherResult[num].motor = 0;
	}
	s_FeatherResult[num].motor += s_FeatherPreset[num].motor;
}

void SetFeathers()
{
	switch(s_DendriteMode)
	{
		case EFrontBack:
		{
			TVector2f servosFront = DecodeStickData(EFrontLeftServo);
			TVector2f motorsFront = DecodeStickData(EFrontLeftMotor);
			TVector2f servosBack  =	DecodeStickData(EBackRightServo);
			TVector2f motorsBack  =	DecodeStickData(EBackRightMotor);
			SetFeatherData(EFrontLeftFeather,	servosFront.a,	motorsFront.a);
			SetFeatherData(EFrontRightFeather,	servosFront.b,	motorsFront.b);
			SetFeatherData(EBackLeftFeather,	servosBack.a,	motorsBack.a);
			SetFeatherData(EBackRightFeather,	servosBack.b,	motorsBack.b);
		}
		break;
		
		case ELeftRight:
		{
			TVector2f servosLeft  = DecodeStickData(EFrontLeftServo);
			TVector2f motorsLeft  = DecodeStickData(EFrontLeftMotor);
			TVector2f servosRight = DecodeStickData(EBackRightServo);
			TVector2f motorsRight = DecodeStickData(EBackRightMotor);
			SetFeatherData(EFrontLeftFeather,	servosLeft.a,	motorsLeft.a);
			SetFeatherData(EFrontRightFeather,	servosRight.a,	motorsRight.a);
			SetFeatherData(EBackLeftFeather,	servosLeft.b,	motorsLeft.b);
			SetFeatherData(EBackRightFeather,	servosRight.b,	motorsRight.b);
		}
		break;
	}
}

char IsStickNearPrevious(TEStics num)
{
	float max = s_stickPrevious[num].x + THRESHOLD;
	float min = s_stickPrevious[num].x - THRESHOLD;
	if(	(s_stickCurrent[num].x < max) && (s_stickCurrent[num].x > min) )
	{
		return 1;
	}
	max = s_stickPrevious[num].y + THRESHOLD;
	min = s_stickPrevious[num].y - THRESHOLD;
	if(	(s_stickCurrent[num].y < max) && (s_stickCurrent[num].y > min) )
	{
		return 1;
	}
	return 0;
}

char IsStickNearZero(TEStics num)
{
	
}

void UpdateDendrite()
{
	for(uint8_t n = 0; n < STICK_NUM; ++n)
	{
		switch(s_DendriteState[n])
		{
			case EStickThresheld:
			{
				if(IsStickNearPrevious(n))
				{
					return;
				}
				else
				{
					s_stickPrevious[n] = s_stickCurrent[n];
				}
			}
			break;
			case EStickShouldSet:
			{
				if(IsStickNearZero(n))
				// Next state will not change data while stick is near
				s_DendriteState[n] = EStickThresheld;
			}
			break;
		}
	}
}
