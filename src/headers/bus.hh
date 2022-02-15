#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>

template <typename T> // T is the bit width of the bus
struct bus_t
{   
    T bitfield;
    char r_perm : 3; // 8/16/32 bit read
    char w_perm : 3; // 8/16/32 bit read
    T (*read_ptr)(struct bus_t<T>, int size);
    void (*write_ptr)(struct bus_t<T>, int size, T data);
};

template <typename T>
T read(struct bus_t<T>, int size); 

template <typename T>
void write(struct bus_t<T>, int size, T data);


class Bus
{
private:

public:
    struct bus_t<uint32_t> BIOS_ROM;
    Bus();
    ~Bus();
};


#endif /* !BUS_H */
    



