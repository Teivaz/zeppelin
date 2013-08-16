#ifndef STREAMBUFFER_H_
#define STREAMBUFFER_H_
#include "config.h"

extern char s_buffer[STREAM_BUFFER_SIZE];
extern char* s_bufferFirst;
extern char* s_bufferLast;

inline void IncrementBufferPointer(char* pointer)
{
	pointer += 1;
	if(pointer == &s_buffer[STREAM_BUFFER_SIZE])
	{
		pointer = &s_buffer[0];
	}
}

inline char GetStreamBufferSize()
{
	if(s_bufferLast >= s_bufferFirst)
	return s_bufferLast - s_bufferFirst;
	
	return (s_buffer + STREAM_BUFFER_SIZE - s_bufferFirst) + (s_bufferLast - &s_buffer[0]);
}

inline void WriteStream(char data)
{
	*s_bufferLast = data;
	IncrementBufferPointer(s_bufferLast);
}

inline char ReadStream()
{
	char ret = *s_bufferFirst;
	IncrementBufferPointer(s_bufferFirst);
	return ret;
}
#endif /* STREAMBUFFER_H_ */