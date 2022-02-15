#include "headers/master.hh"
#include "headers/bus.hh"
#include <memory.h>
#include <stdint.h>
#include <iostream>

int main()
{
    int size = 4;
    /*
    std::unique_ptr<BIOS_ROM_Bus> b01(new BIOS_ROM_Bus);
    std::unique_ptr<WRAM32K_Bus> b02(new WRAM32K_Bus);
    std::unique_ptr<IO_Bus> b03(new IO_Bus);
    std::unique_ptr<OAM_Bus> b04(new OAM_Bus);
    std::unique_ptr<WRAM256K_Bus> b05(new WRAM256K_Bus);
    std::unique_ptr<PRAM_Bus> b06(new PRAM_Bus);
    std::unique_ptr<VRAM_Bus> b06(new VRAM_Bus);
    std::unique_ptr<GamePak_ROM_Bus> b06(new GamePak_ROM_Bus);
    std::unique_ptr<GamePak_Flash_Bus> b06(new GamePak_Flash_Bus);
    std::unique_ptr<GamePak_SRAM_Bus> b06(new GamePak_SRAM_Bus);
    */
    Bus prova;
    printf("%p\n", (void *)prova.BIOS_ROM.read_ptr);
    //prova.BIOS_ROM.*read_ptr(prova.BIOS_ROM, size);
    //prova.BIOS_ROM.bitfield = 534;
    //int su = (prova.BIOS_ROM.*read_ptr)(prova.BIOS_ROM, size);

    // piedi.lookup[Bus::BIOS_ROM].bitfield = 534; // 10 0001 0110
    // printf("%d\n", piedi.BIOS_ROM.bitfield); // 534;
    // printf("%d\n", piedi.BIOS_ROM.read_ptr(piedi.BIOS_ROM, size)); // dovrebbe tornare 2
}