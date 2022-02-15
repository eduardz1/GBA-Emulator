#ifndef BUS_H
#define BUS_H

#include <stdint.h>

template <typename T>//T is the bit width of the bus
class Bus
{
    
public:
    T bitfield;
    virtual void f() = 0; // dumb code
    
    int read(Bus* address,int flag);
};

template <typename T>
class BIOS_ROM_Bus : public Bus
{
public:
    uint32_t bitfield;
    
    BIOS_ROM_Bus();
    ~BIOS_ROM_Bus();
};

template <typename T>
class WRAM32K_Bus : public  Bus
{
public:
    uint32_t bitfield;
    WRAM32K_Bus();
    ~WRAM32K_Bus();
};

template <typename T>
class IO_Bus : public Bus
{
public:
    uint32_t bitfield;
    IO_Bus();
    ~IO_Bus();
};

template <typename T>
class OAM : public Bus
{
public:
    uint32_t bitfield;
    OAM();
    ~OAM();
};

template <typename T>
class WRAM256K_Bus : public Bus
{
public:
    uint16_t bitfield;
    int Bus::read(WRAM256K_Bus* address,int flag);
    WRAM256K_Bus();
    ~WRAM256K_Bus();
};

template <typename T>
class PRAM_Bus : public Bus
{
public:
    uint16_t bitfield;
    PRAM_Bus();
    ~PRAM_Bus();
};

template <typename T>
class VRAM_Bus : public Bus
{
public:
    uint16_t bitfield;
    VRAM_Bus();
    ~VRAM_Bus();
};

template <typename T>
class GamePak_ROM_Bus : public Bus
{
public:
    uint16_t bitfield;
    GamePak_ROM_Bus();
    ~GamePak_ROM_Bus();
};

template <typename T>
class GamePak_Flash_Bus : public Bus
{
public:
    uint16_t bitfield;
    GamePak_Flash_Bus();
    ~GamePak_Flash_Bus();
};

template <typename T>
class GamePak_SRAM_Bus : public Bus
{
public:
    uint8_t bitfield;
    GamePak_SRAM_Bus();
    ~GamePak_SRAM_Bus();
};

#endif /* !BUS_H */
    