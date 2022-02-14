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
};

extern Random64 random;

inline uint64 randomFocusedOnZero(const uint64 &seed, const double &stretch = 1)
{
    return random(0, seed * stretch) / random(1, seed / 2);
}

inline uint64 randomFocusedOnCenter(const uint64 &seed)
{
    return random(0, seed / 2) + random(0, seed / 2);
}