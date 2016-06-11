#ifndef MOTOR_H_
#define MOTOR_H_

void Motor_Tick();

// Call when timers are disabled (probably)
void Motor_InitTimer();

// DC Motor
// -128 - max CCW, 0 - stop, 127 - max CW
void Motor_SetSpeedSigned(signed char speed);

// 0 - max CCW, 127 - stop, 255 - max CW
void Motor_SetSpeedUnsigned(unsigned char speed);

#endif /* MOTOR_H_ */