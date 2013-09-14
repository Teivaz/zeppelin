#ifndef STREAMBUFFER_H_
#define STREAMBUFFER_H_
#include "config.h"

typedef struct 
{
	char buffer[STREAM_BUFFER_SIZE];
	char* first;
	char* last;
} TStreamBuffer;

inline void InitializeStream(TStreamBuffer* stream)
{
	stream->first = stream->buffer;
	stream->last = stream->buffer;
}

inline void IncrementFirst(TStreamBuffer* stream)
{
	++stream->first;
	if(stream->first == stream->buffer + STREAM_BUFFER_SIZE)
	{
		stream->first = stream->buffer;
	}
}

inline void IncrementLast(TStreamBuffer* stream)
{
	++stream->last;
	if(stream->last == stream->buffer + STREAM_BUFFER_SIZE)
	{
		stream->last = stream->buffer;
	}
}

inline char GetStreamBufferSize(TStreamBuffer* stream)
{
	return (stream->first != stream->last);
	/*
	if(stream->last >= stream->first)
	{
		return (stream->last - stream->first);
	}
	
	return (stream->buffer + STREAM_BUFFER_SIZE - stream->first) + (stream->last - stream->buffer);*/
}

inline void WriteStream(TStreamBuffer* stream, char data)
{
	*(stream->last) = data;
	IncrementLast(stream);
}

inline char ReadStream(TStreamBuffer* stream)
{
	char ret = *stream->first;
	IncrementFirst(stream);
	return ret;
}
#endif /* STREAMBUFFER_H_ */