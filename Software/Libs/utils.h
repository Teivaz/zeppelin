#ifndef _UTILS_H_
#define _UTILS_H_

#define SET_BIT(REG, BIT)     ((REG) |= (1<<BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1<<BIT))

#define READ_BIT(REG, BIT)    ((REG) & (1<<BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

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

#endif /* _UTILS_H_ */