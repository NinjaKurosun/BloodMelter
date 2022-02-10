#include "Rand64.h"
#include <Windows.h>

Random64 random(GetTickCount64());

Random64::Random64(uint64 seed/*= 0*/) :X(seed) 
{
}
Random64& Random64::operator=(uint64 seed) 
{
	X = seed; return *this; 
}

uint64 Random64::operator()(uint64 seed /*= uint64(-1)*/)
{
    const uint64 a = 3202034522624059733ULL;
    const uint64 c = 1ULL;

    if (seed != uint64(-1)) X = seed;
    uint64 Y = a * X + c;
    X = a * Y + c;
    Y = (Y & 0xFFFFFFFF00000000ULL) | (X >> 32);
    return Y;
}

uint64 Random64::operator()(uint64 min, uint64 max)
{
    return (*this)() % (max - min) + min;
}