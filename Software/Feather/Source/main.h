#ifndef MAINAVR_H_
#define MAINAVR_H_

int main();
void Init();

void SetMotorSpeedSigned(signed char speed);
void SetMotorSpeedUnsigned(unsigned char speed);

void SetServoPosition(unsigned char position);
void AdvanceServoState();
 
void ResetSpiTimer();
void ReadSpi();

typedef enum
{
	EServoFirst,
	EServoSecond,
	EServoPause
}	TServoState;

// Configuration
#define SERVO_PIN	PB1
#define MOTOR_PIN_A PB3
#define MOTOR_PIN_B	PB4
// SPI uses hardware pins
//#define MOSI_PIN	PB0
//#define CLK_PIN	PB2

#endif /* MAINAVR_H_ */