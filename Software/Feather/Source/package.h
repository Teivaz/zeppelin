#ifndef PACKAGE_H_
#define PACKAGE_H_

char Package_GetDataByte(uint8_t num);
char Package_IsValid();
void Package_Reset();
void Package_AddByte(char byte);

#define c_packageLength 5
char s_packageValid;
char s_packageLength;
char s_package[c_packageLength];

#include "package.cpp"

#endif /* PACKAGE_H_ */