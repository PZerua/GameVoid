#pragma once

#include "Header.h"

BYTE bitSet(BYTE byte, int bit);
BYTE bitReset(BYTE byte, int bit);
bool testBit(BYTE byte, int bit);
BYTE bitGetVal(BYTE byte, int bit);