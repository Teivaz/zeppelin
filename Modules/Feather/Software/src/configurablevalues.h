#ifndef _CONFIGURABLEVALUES_H_INCLUDED_
#define _CONFIGURABLEVALUES_H_INCLUDED_

#include "protocol.h"

void initCv();

uint8_t readCv(PZ_Feather_CV);
void writeCv(PZ_Feather_CV, void* value);

void resetCv(PZ_Feather_CV);
void resetAllCv();

void setAddress(uint8_t value);
void setMotorDef(int8_t value);
void setMotorCal1(int8_t value);
void setMotorCal2(int8_t value);
void setMotorCal0(int8_t value);
void setServoDef(uint8_t value);
void setServoCal1(uint8_t value);
void setServoCal2(uint8_t value);

uint8_t getAddress();
int8_t getMotorDef();
int8_t getMotorCal1();
int8_t getMotorCal2();
int8_t getMotorCal0();
uint8_t getServoDef();
uint8_t getServoCal1();
uint8_t getServoCal2();

// Weak Callbacks
void CvOnAddressChanged(uint8_t value);
void CvOnMotorDefChanged(int8_t value);
void CvOnMotorCal1Changed(int8_t value);
void CvOnMotorCal2Changed(int8_t value);
void CvOnMotorCal0Changed(int8_t value);
void CvOnServoDefChanged(uint8_t value);
void CvOnServoCal1Changed(uint8_t value);
void CvOnServoCal2Changed(uint8_t value);

#endif // _CONFIGURABLEVALUES_H_INCLUDED_
