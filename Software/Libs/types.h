#ifndef TYPES_H_
#define TYPES_H_

#ifndef __cplusplus
typedef enum {false = 0, true = !false} bool;
#endif


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

#endif /* TYPES_H_ */