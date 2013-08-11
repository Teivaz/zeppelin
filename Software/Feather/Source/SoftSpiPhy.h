#ifndef _SOFTSPIPHY_H_
#define _SOFTSPIPHY_H_

// SPI configuration
#define SPI_CPOL 0				// Clock polarity
#define SPI_CPHA 0				// Clock phase
#define SPI_WORD_SIZE 8		// Size of SPI word

#define _SPI_OPTIMIZE

// -------------- public functions --------------
void SpiClkIrq(void); // Enter CLK pin interrupt here
char GetSpiReceivedWord(void);

// -------------- private functions --------------
void _SpiReadBit(void);
void _SpiWriteBit(void);

// SPI data
extern char s_spiReadBuffer;
extern short s_spiReadBufferSize;
extern char s_spiWriteBuffer;
extern short s_spiWriteBufferSize;

#endif //_SOFTSPIPHY_H_
