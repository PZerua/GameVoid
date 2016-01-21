#include "Header.h"

BYTE bitSet(const BYTE &byte, const int &bit)
{
	BYTE tempByte = byte | (0x01 << bit);
	return tempByte;
}

BYTE bitReset(const BYTE &byte, const int &bit)
{
	BYTE tempByte = byte & ~(0x01 << bit);
	return tempByte;
}

bool testBit(const BYTE &byte, const int &bit)
{
	return ((byte & (0x01 << bit)) == (0x01 << bit));
}

BYTE bitGetVal(const BYTE &byte, const int &bit)
{
	BYTE tempByte = (byte & (0x01 << bit)) >> bit;
	return tempByte;
}
