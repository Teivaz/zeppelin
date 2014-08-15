#ifndef PACKAGE_H_
#define PACKAGE_H_

char Package_GetDataByte(uint8_t num);
char Package_IsValid();
inline void Package_Reset();
void Package_AddByte(char byte);

void Package_Ping(); // Resets dead time

#define c_maxDeadTicks 3
#define c_packageLength 5

#include "package.cpp"

#endif /* PACKAGE_H_ */