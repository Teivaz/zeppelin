#ifndef MAINAVR_H_
#define MAINAVR_H_

#include "types.h"

int main(void);
void Init();


inline bool ReadSpiDataBit(void)
{
	return false;
}

inline void WriteSpiDataBit(bool bit)
{
	;
}

#endif /* MAINAVR_H_ */