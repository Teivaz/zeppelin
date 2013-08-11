#ifndef MAINAVR_H_
#define MAINAVR_H_

#include "types.h"

int main(void);
void Init();


extern inline bool ReadSpiDataBit(void)
{
	return false;
}

extern inline void WriteSpiDataBit(bool bit)
{
	;
}

#endif /* MAINAVR_H_ */