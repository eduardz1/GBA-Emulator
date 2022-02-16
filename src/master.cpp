#include "headers/master.hh"
#include "headers/bus.hh"
#include "headers/arm7tdmi.hh"
#include <memory.h>
#include <stdint.h>
#include <iostream>
#include <functional>

int main()
{
    int size = 32;
    uint32_t write_try = 98989898;
    int checks = 17000;
    Bus prova;
    cpu::Arm7tdmi cpu;
    (prova.*prova.BIOS_ROM.write_ptr)(prova.BIOS_ROM, size, write_try);
    std::cout << "----------------------------------------------" << std::endl;
    uint32_t su = (prova.*prova.BIOS_ROM.read_ptr)(prova.BIOS_ROM, size);
    std::cout << "Valore:" << (uint)su << std::endl;
}