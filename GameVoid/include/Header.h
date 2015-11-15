#pragma once

#include <iostream>
#include <fstream>

using namespace std;

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

// In C++ char has 1 Byte width
// It has to be unsigned because we need up to 255 values
// In signed types 1 bit is used to denote sign
typedef unsigned char BYTE;

// Short has 2 Bytes width
typedef unsigned short WORD;
