#pragma once
#include <cstdint>
#include <array>
class Memory{

    public:
        //RAM of 4 MB = 2^32 = width of the bus
        std::array<uint8_t, 4*1024*1024*1024> RAM;

    private:

};
