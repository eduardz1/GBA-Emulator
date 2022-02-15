#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stdio.h>

template <typename T>//T is the bit width of the bus
class Bus
{   
public:
    T bitfield;   
    int read();
};

template <typename T>
class BIOS_ROM_Bus : public Bus<uint32_t>
{
public:
    uint32_t bitfield;
    
    BIOS_ROM_Bus();
    ~BIOS_ROM_Bus();
};

template <typename T>
class WRAM32K_Bus : public  Bus<uint32_t>
{
public:
    uint32_t bitfield;
    WRAM32K_Bus();
    ~WRAM32K_Bus();
};

template <typename T>
class IO_Bus : public Bus<uint32_t>
{
public:
    uint32_t bitfield;
    IO_Bus();
    ~IO_Bus();
};

template <typename T>
class OAM_Bus : public Bus<uint32_t>
{
public:
    uint32_t bitfield;
    OAM_Bus();
    ~OAM_Bus();
};

template <typename T>
class WRAM256K_Bus : public Bus<uint16_t>
{
public:
    uint16_t bitfield;
    WRAM256K_Bus() {}
    ~WRAM256K_Bus(){}
    //int Bus<uint16_t>::read();
};

template <typename T>
class PRAM_Bus : public Bus<uint16_t>
{
public:
    uint16_t bitfield;
    PRAM_Bus();
    ~PRAM_Bus();
};

template <typename T>
class VRAM_Bus : public Bus<uint16_t>
{
public:
    uint16_t bitfield;
    VRAM_Bus();
    ~VRAM_Bus();
};

template <typename T>
class GamePak_ROM_Bus : public Bus<uint16_t>
{
public:
    uint16_t bitfield;
    GamePak_ROM_Bus();
    ~GamePak_ROM_Bus();
};

template <typename T>
class GamePak_Flash_Bus : public Bus<uint16_t>
{
public:
    uint16_t bitfield;
    GamePak_Flash_Bus();
    ~GamePak_Flash_Bus();
};

template <typename T>
class GamePak_SRAM_Bus : public Bus<uint8_t>
{
public:
    uint8_t bitfield;
    GamePak_SRAM_Bus();
    ~GamePak_SRAM_Bus();
};

#endif /* !BUS_H */
    