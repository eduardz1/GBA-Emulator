#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>

class Bus
{
public: // should be made private after

    template <typename T> // T is the bit width of the bus
    struct bus_t
    {
        T bitfield;
        char r_perm : 3; // 32/16/8 bit read -> 0b001 equals to permit to read on 8 bit, 0b101 => permit to read on 32 and 8 bit
        char w_perm : 3; // 32/16/8 bit write
        uint32_t min_range,max_range; //minimum and maximum addressable range for a given bus

        T (Bus::*read_ptr)(struct bus_t<T> bus, int size);
        void (Bus::*write_ptr)(struct bus_t<T> bus, int size, T data);
    };

public:
    template <typename T>
    T read(struct bus_t<T> bus, int size);

    template <typename T>
    void write(struct bus_t<T> bus, int size, T data);

    template <typename T>
    bool check_constraint(struct bus_t<T> bus, uint address);

public:
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> WRAM_32K;
    struct bus_t<uint32_t> IO;
    struct bus_t<uint32_t> OAM;
    struct bus_t<uint16_t> WRAM_256K;
    struct bus_t<uint16_t> P_RAM;
    struct bus_t<uint16_t> VRAM;
    struct bus_t<uint16_t> GamePak_ROM;
    struct bus_t<uint16_t> GamePak_Flash;
    struct bus_t<uint8_t> GamePak_SRAM;

    //RAM of 4 MB = 2^32 = width of the bus
    std::array<uint8_t, 4*1024*1024> RAM;

public:
    Bus();
    ~Bus();
};

#include "bus.tpp"
#endif /* !BUS_H */
