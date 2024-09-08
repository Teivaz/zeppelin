#ifndef _CONFIGURABLEVALUES_H_INCLUDED_
#define _CONFIGURABLEVALUES_H_INCLUDED_

#include "protocol.h"

void initCv();

uint8_t readCv(PZ_Keel_CV);
void writeCv(PZ_Keel_CV, void const* value);

void resetCv(PZ_Keel_CV);
void resetAllCv();

void setAddress(uint8_t value);

uint8_t getAddress();

#endif // _CONFIGURABLEVALUES_H_INCLUDED_
