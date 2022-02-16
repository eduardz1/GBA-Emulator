#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>

class Bus
{
private: // should be made private after

    template <typename T> // T is the bit width of the bus
    struct _bus
    {
        T bitfield;
        char r_perm : 3; // 32/16/8 bit read
        char w_perm : 3; // 32/16/8 bit write
        uint32_t min_range,max_range; //minimum and maximum addressable range for a given bus

        T (Bus::*read_ptr)(struct _bus<T>, int);
        void (Bus::*write_ptr)(struct _bus<T>, int, T);
    };

public:
    template <typename T>
    T read(struct _bus<T> bus, int bit_length);

    template <typename T>
    void write(struct _bus<T> bus, int bit_length, T data);

    template <typename T>
    bool check_constraint(struct _bus<T> bus, uint address);

public:
    struct _bus<uint32_t> BIOS_ROM;
    struct _bus<uint32_t> WRAM_32K;
    struct _bus<uint32_t> IO;
    struct _bus<uint32_t> OAM;
    struct _bus<uint16_t> WRAM_256K;
    struct _bus<uint16_t> P_RAM;
    struct _bus<uint16_t> VRAM;
    struct _bus<uint16_t> GamePak_ROM;
    struct _bus<uint16_t> GamePak_Flash;
    struct _bus<uint8_t>  GamePak_SRAM;

    //RAM of 4 MB = 2^32 = width of the bus
    std::array<uint8_t, 4*1024*1024> RAM;

public:
    Bus();
    ~Bus();
};

#include "bus.tpp"
#endif /* !BUS_H */
