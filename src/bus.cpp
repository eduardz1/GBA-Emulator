#include "headers/bus.hh"
//TODO: Usare macro per i permessi 
Bus::Bus()
{  
    // General Internal Memory
    BIOS_ROM      = {0, 0b111, 0b000, 0x00000000, 0x00003FFF, &Bus::read<uint32_t>, &Bus::write<uint32_t>};
    WRAM_256K     = {0, 0b111, 0b111, 0x02000000, 0x0203FFFF, &Bus::read<uint16_t>, &Bus::write<uint16_t>};
    WRAM_32K      = {0, 0b111, 0b111, 0x03000000, 0x03007FFF, &Bus::read<uint32_t>, &Bus::write<uint32_t>};
    IO            = {0, 0b111, 0b111, 0x04000000, 0x040003FE, &Bus::read<uint32_t>, &Bus::write<uint32_t>};
    // Internal Display Memory
    P_RAM         = {0, 0b111, 0b110, 0x05000000, 0x050003FF, &Bus::read<uint16_t>, &Bus::write<uint16_t>};
    VRAM          = {0, 0b111, 0b110, 0x06000000, 0x06017FFF, &Bus::read<uint16_t>, &Bus::write<uint16_t>};
    OAM           = {0, 0b111, 0b110, 0x07000000, 0x070003FF, &Bus::read<uint32_t>, &Bus::write<uint32_t>};
    // GamePak Memory(the cartridge basically)
    GamePak_ROM   = {0, 0b111, 0b000, 0x08000000, 0x0DFFFFFF, &Bus::read<uint16_t>, &Bus::write<uint16_t>};
    GamePak_Flash = {0, 0b111, 0b110, 0x08000000, 0x0DFFFFFF, &Bus::read<uint16_t>, &Bus::write<uint16_t>};
    GamePak_SRAM  = {0, 0b001, 0b001, 0x0E000000, 0x0E00FFFF, &Bus::read<uint8_t>,  &Bus::write<uint8_t> };

    // Clearing RAM contents
    printf("Dimensione RAM:%ld\n",RAM.size());
    for(auto i = 0; i<RAM.size(); i++)      
        RAM[i] = 0x1;
}

Bus::~Bus()
{
}
