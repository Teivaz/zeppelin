#ifndef _UTILS_H_
#define _UTILS_H_

#define SET_BIT(REG, BIT)     ((REG) |= (1<<BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1<<BIT))

#define READ_BIT(REG, BIT)    ((REG) & (1<<BIT))

#define CLEAR_REG(REG)        ((REG) ^= (REG))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         (REG)

#define TOGGLE_BIT(REG, BIT)  ((REG) ^ (1<<BIT))

#define MSB(BIT)              ((BIT >> 7) & 0x1)

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

inline char HASH(char* data, unsigned char size)
{
	char ret = 0;
	for(unsigned char i = 0; i < size; ++i)
	{
		ret ^= data[i];
	}
	return ret;
}

inline char HASH4(char* data)
{
	return data[0] ^ data[1] ^ data[2] ^ data[3];
}

inline char HASH2(char* data)
{
	return data[0] ^ data[1];
}

#ifdef _USE_ABS
#define abs(val) (val > 0 ? val : -val)
#endif

#ifdef _USE_MINMAX
#define min(x, y) (x > y ? y : x)
#define max(x, y) (x < y ? y : x)
#endif

#define nop() __asm__ __volatile__ ("nop"::)

#endif /* _UTILS_H_ */