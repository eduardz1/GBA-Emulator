#include "headers/master.hh"
#include "headers/bus.hh"
#include <memory.h>
#include <stdint.h>
#include <iostream>

int main()
{
    
    WRAM256K_Bus piedi;
    piedi.bitfield = 7;
    printf("%d\n",piedi.read(piedi));
}