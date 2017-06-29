#include "Header.h"

BYTE bitSet(BYTE byte, int bit)
{
	BYTE tempByte = byte | (0x01 << bit);
	return tempByte;
}

BYTE bitReset(BYTE byte, int bit)
{
	BYTE tempByte = byte & ~(0x01 << bit);
	return tempByte;
}

bool testBit(BYTE byte, int bit)
{
	return ((byte & (0x01 << bit)) == (0x01 << bit));
}

BYTE bitGetVal(BYTE byte, int bit)
{
	BYTE tempByte = (byte & (0x01 << bit)) >> bit;
	return tempByte;
}
