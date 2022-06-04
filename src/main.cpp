#include "cpu/include/main.hh"
#include "cpu/include/bus.hh"
#include "cpu/include/arm7tdmi.hh"
#include <memory.h>
#include <cstdint>
#include <iostream>
#include <functional>

int main()
{
    int size = 32;
    //uint32_t write_try = 98989898;
    //int checks = 17000;
    Bus prova;
    Arm7tdmi cpu;
    FILE* bios_bin=fopen("gba_bios.bin","rb");
    prova.init_bios_ram(bios_bin);
    //(prova.*prova.BIOS_ROM.write_ptr)(prova.BIOS_ROM, size, write_try);
    //std::cout << "----------------------------------------------" << std::endl;
    //uint32_t su = (prova.*prova.BIOS_ROM.read_ptr)(prova.BIOS_ROM, size);
    //std::cout << "Valore:" << (uint)su << std::endl;

    for(int i=prova.BIOS_ROM.min_range;i<prova.BIOS_ROM.max_range;i = i+4){
        // cpu::Arm7tdmi::_instruction tmp;
        // tmp.word  = prova.RAM[i];
        // tmp.word |= prova.RAM[i + 1] << 8;
        // tmp.word |= prova.RAM[i + 2] << 16;
        // tmp.word |= prova.RAM[i + 3] << 24;

        // cpu.decode_execute(tmp);
    }
    /*cpu::Arm7tdmi::_instruction tmp;
    tmp.word=0xE3A000DF;
    cpu.decode_execute(tmp);*/
    fclose(bios_bin);//ridimmi ridimmi ridimmi
}