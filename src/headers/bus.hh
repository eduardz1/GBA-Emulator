#ifndef BUS_H
#define BUS_H

#include <stdint.h>

class Bus
{
public:
    virtual void f() = 0; // dumb code
};

class BIOS_ROM_Bus : Bus
{
public:
    uint32_t bitfield;
    void f()
    {
        return;
    }
};

class WRAM32K_Bus : Bus
{
public:
    uint32_t bitfield;
};

class IO_Bus : Bus
{
public:
    uint32_t bitfield;
};

class OAM : Bus
{
public:
    uint32_t bitfield;
};

class WRAM256K_Bus : Bus
{
public:
    uint16_t bitfield;
};

class PRAM_Bus : Bus
{
public:
    uint16_t bitfield;
};

class VRAM_Bus : Bus
{
public:
    uint16_t bitfield;
};

class GamePak_ROM_Bus : Bus
{
public:
    uint16_t bitfield;
};

class GamePak_Flash_Bus : Bus
{
public:
    uint16_t bitfield;
};

class GamePak_SRAM_Bus : Bus
{
public:
    uint8_t bitfield;
};

#endif /* !BUS_H */