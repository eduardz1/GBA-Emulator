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
        char r_perm : 3; // 8/16/32 bit read
        char w_perm : 3; // 8/16/32 bit read

        T (Bus::*read_ptr)(struct bus_t<T> bus, int size);
        void (Bus::*write_ptr)(struct bus_t<T> bus, int size, T data);
    };

    template <typename T>
    T read(struct bus_t<T> bus, int size);

    template <typename T>
    void write(struct bus_t<T> bus, int size, T data);


    struct bus_t<uint32_t> BIOS_ROM;

public:
    Bus();
    ~Bus();

    /*struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;
    struct bus_t<uint32_t> BIOS_ROM;*/
};

#include "bus.tpp"
#endif /* !BUS_H */
