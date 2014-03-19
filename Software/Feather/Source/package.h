#ifndef PACKAGE_H_
#define PACKAGE_H_

const uint8_t c_packageLength = 5;
char s_packageValid;
char s_packageLength;
char s_package[c_packageLength];

char Package_GetDataByte(uint8_t num);
char Package_IsValid();
void Package_Reset();
void Package_AddByte(char byte);

#endif /* PACKAGE_H_ */