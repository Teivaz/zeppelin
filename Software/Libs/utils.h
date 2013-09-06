#ifndef _UTILS_H_
#define _UTILS_H_

#define SET_BIT(REG, BIT)     ((REG) |= (1<<BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1<<BIT))

#define READ_BIT(REG, BIT)    ((REG) & (1<<BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define TOGGLE_BIT(REG, BIT)	(READ_BIT(REG, BIT) ? CLEAR_BIT(REG, BIT) : SET_BIT(REG, BIT))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

inline char CRC(char* data, uint8_t size)
{
	char ret = 0;
	for(uint8_t i = 0; i < size; ++i)
	{
		ret += data[i];
	}
	return ret;
}

#ifdef _USE_ABS
#define abs(val) (val > 0 ? val : -val)
#endif

#ifdef _USE_MINMAX
#define min(x, y) (x > y ? y : x)
#define max(x, y) (x < y ? y : x)
#endif

#endif /* _UTILS_H_ */