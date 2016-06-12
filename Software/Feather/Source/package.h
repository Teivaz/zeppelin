#ifndef PACKAGE_H_
#define PACKAGE_H_

void Package_Init();

// Interrupt. When SPI has byte
void PackageI_OnReceived(char b);

// return 1 if has unread data
char Package_IsDirty();

// call this when package is dirty
void Package_Process();

// Will return pointer to data if data is present. Otherwise will return 0
char* Package_GetPayload();

void Package_ClearState();

#endif /* PACKAGE_H_ */