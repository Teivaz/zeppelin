#ifndef _DENDRITE_H_INCLUDED_
#define _DENDRITE_H_INCLUDED_

// Dendrite is a communication interface facing Feather modules

#include "protocol.h"

void dendriteSend(PZ_Package const*);

/// @return the number of bytes that are ready to be read
uint8_t getDendriteDataLen();

/// @return the data. Only read as much data as getDendriteDataLen() has
uint8_t* getDendriteData();

void resetDendriteData();

#endif // _DENDRITE_H_INCLUDED_
