#ifndef BUFFER_H_
#define BUFFER_H_

extern void *memset(void *, int, size_t);

/*
0| [P] [S] [1] [2] [C] [ ] [ ] [ ] [ ] [ ]
1| [ ] [P] [S] [1] [2] [C] [ ] [ ] [ ] [ ]
2| [ ] [ ] [P] [S] [1] [2] [C] [ ] [ ] [ ]
3| [ ] [ ] [ ] [P] [S] [1] [2] [C] [ ] [ ]
4| [ ] [ ] [ ] [ ] [P] [S] [1] [2] [C] [ ]
0| [ ] [ ] [ ] [ ] [ ] [P] [S] [1] [2] [C]
    0   1   2   3   4   5   6   7   8   9
*/

#define PACKAGE_LENGTH 5
#define TOTAL_PACKAGES PACKAGE_LENGTH
#define BUFFER_LENGTH (PACKAGE_LENGTH*2)

typedef struct  
{
	char primary;		// Primary letter
	char secondary;		// Secondary letter
	char payload[2];	// Payload
	char crc;			// Checksum
} Package;

typedef struct
{
	char data[BUFFER_LENGTH];		// Bytes of package
	unsigned char length;
	unsigned char prev_length;
} Buffer;

inline void Buffer_Init(Buffer* buf)
{
	memset(buf->data, 0, BUFFER_LENGTH);
	buf->length = 0;
	buf->prev_length = 0;
}

// Will push symbol if length >= 0 and increase length of buffer
inline void Buffer_Push(Buffer* buf, char symbol)
{
	buf->data[buf->length] = symbol;
	buf->data[buf->length + PACKAGE_LENGTH] = symbol;
	buf->length = (buf->length + 1) % PACKAGE_LENGTH;
}

inline char Buffer_HasData(Buffer* buf)
{
	return buf->length != buf->prev_length;
}

inline void Buffer_MarkRead(Buffer* buf)
{
	buf->prev_length = buf->length;
}

// Will check single package. Returns 0 if no data detected.
// Returns pointer to data if package was detected
inline char* Buffer_Read(Buffer* buf)
{
	// increase prev_length
	buf->prev_length = (buf->prev_length + 1) % PACKAGE_LENGTH;
	
	// use prev_length as package id because it is matches exactly for length of 5
	Package* package = (Package*)(buf->data + buf->prev_length);
	if (package->primary != PRIMARY_LETTER)
		return 0;
	else if (package->secondary != SECONDARY_LETTER)
		return 0;
	else if (CRC4((char*)package) != package->crc)
		return 0;
	else
		return package->payload;
}

inline char Buffer_IsValidPackage(Buffer* buf, unsigned char id)
{
	id %= TOTAL_PACKAGES;
}

#endif /* BUFFER_H_ */