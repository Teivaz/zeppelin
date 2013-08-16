#ifndef TRANSMITPOOL_H_
#define TRANSMITPOOL_H_

typedef struct
{
	const char* pointer;
	char size;
} TPool;

inline char IsDone(TPool pool)
{
	return pool.size == 0;
}

inline char PoolPopFront(TPool pool)
{
	if(IsDone(pool))
		return 0;
	--pool.size;	
	return *(pool.pointer++);
}

#endif /* TRANSMITPOOL_H_ */