#pragma once

typedef unsigned long long uint64;

class Random64
{
    uint64 X;

public:
    typedef uint64 RandomValue;
    Random64(uint64 seed = 0);
    Random64& operator=(uint64 seed);

    uint64 operator()(uint64 seed = uint64(-1));
    uint64 operator()(uint64 min, uint64 max);

    uint64 randomFocusedOnZero(const uint64& min, const uint64& max, const double& stretch = 1);
    uint64 randomFocusedOnCenter(const uint64& min, const uint64& max);
};

