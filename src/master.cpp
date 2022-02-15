#include "headers/master.hh"
#include "headers/bus.hh"

int main()
{
    bus_init();
    WRAM256K_Bus<uint16_t> piedi;
    piedi.read(&piedi,0);
}