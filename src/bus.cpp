#include "headers/bus.hh"

template <typename T>
T read(struct bus_t<T> bus,int size){
    printf("Bus.bitfield is %d\n", bus.bitfield);
    printf("Size is %d\n", size);
    return bus.bitfield >> size; // 10 0001 = 33
}

template <typename T>
void write(struct bus_t<T> bus, int size, T data){
    bus.bitfield = data;
}

Bus::Bus() {
    BIOS_ROM.r_perm = 7;
    BIOS_ROM.w_perm = 0;
    BIOS_ROM.read_ptr = read<uint32_t>;
}

Bus::~Bus(){

}