#pragma once

#include "gb/data_types.h"

namespace utils
{

BYTE bitSet(BYTE byte, int bit);
BYTE bitReset(BYTE byte, int bit);
bool testBit(BYTE byte, int bit);
BYTE bitGetVal(BYTE byte, int bit);

} // namespace utils