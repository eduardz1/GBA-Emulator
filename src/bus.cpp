#include "headers/bus.hh"

Bus::Bus()
{
    // auto fp = &read<uint32_t>;
    /*BIOS_ROM =
    {
        0,
        7,
        0,
        &read<uint32_t>,
        &write<uint32_t>
    };*/

    BIOS_ROM.bitfield = 0;
    BIOS_ROM.r_perm = 7;
    BIOS_ROM.read_ptr = &Bus::read<uint32_t>;
    BIOS_ROM.write_ptr = &Bus::write<uint32_t>;

    printf("%p\n", (void *)BIOS_ROM.read_ptr);

    read(BIOS_ROM, 4);
    
    //(this->BIOS_ROM->*read<uint32_t>)(BIOS_ROM, 4);

    //lookup = {BIOS_ROM};
    // BIOS_ROM.r_perm = 7;
    // BIOS_ROM.w_perm = 0;
    // BIOS_ROM.read_ptr = read<uint32_t>;
}

Bus::~Bus()
{
}
