#include "headers/master.hh"
#include "headers/bus.hh"
#include <memory>

int main()
{
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
    bus_init();
    WRAM256K_Bus<uint16_t> piedi;
    piedi.
}