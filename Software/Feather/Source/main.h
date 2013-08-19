#ifndef MAINAVR_H_
#define MAINAVR_H_

int main();
void Init();

void SetMotorSpeedSigned(signed char speed);
void SetMotorSpeedUnsigned(unsigned char speed);

void SetServoPosition(unsigned char position);
void ServoNextState();

void ReadSpi();

// Configuration
#define SERVO_PIN	PB1
#define MOTOR_PIN_A PB3
#define MOTOR_PIN_B	PB4
// SPI uses hardware pins

#endif /* MAINAVR_H_ */