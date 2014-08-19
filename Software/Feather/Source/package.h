#ifndef PACKAGE_H_
#define PACKAGE_H_

void Package_Init();
void PackageI_OnReceived(char b);
char Package_IsDirty();
void Package_Process();
char Package_PayloadDetected();
char Package_GetData(char b);
void Package_ResetAllBuffers();

#include "package.cpp"

#endif /* PACKAGE_H_ */