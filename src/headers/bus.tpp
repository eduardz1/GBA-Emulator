#pragma once
template <typename T>
T Bus::read(struct bus_t<T> bus, int size)
{
    T tmp=0;
    switch(size){
        //size 8 bit: I read the first byte
        case 8:
            tmp=RAM[bus.bitfield];
            break;
        //size 16 bit: I read the first two bytes
        case 16:
            tmp=RAM[bus.bitfield+1];
            tmp|=RAM[bus.bitfield]<<8;
            break;
        //size 32 bit: I read the first four bytes
        case 32:
            tmp=RAM[bus.bitfield+3];
            tmp|=RAM[bus.bitfield+2]<<8;
            tmp|=RAM[bus.bitfield+1]<<16;
            tmp|=RAM[bus.bitfield]<<24;   
            break;
    
        default:
            printf("Trying to read from a bus with incorrect width\n");
    }
    printf("Bus.bitfield is %d\n", bus.bitfield);
    printf("Size is %d\n", size);
    return tmp;
}
/*template uint32_t Bus::read(struct bus_t<uint32_t> bus, int size);
template uint16_t Bus::read(struct bus_t<uint16_t> bus, int size);
template uint8_t Bus::read(struct bus_t<uint8_t> bus, int size);
*/
template <typename T>
void Bus::write(struct bus_t<T> bus, int size, T data)
{
    T tmp=0;

    switch(size){
        //size 8 bit: writing 1 byte
        case 8:
            RAM[bus.bitfield]=data;
            break;

        //size 16 bit: writing 2 byte
        case 16:
            RAM[bus.bitfield+1]=data & 0xFF;
            RAM[bus.bitfield]=(data & 0xFF00)>>8;
            break;

        //size 32 bit: writing 4 bytes
        case 32:
            std::cout<<"Dato da inserire:"<<data<<std::endl;

            RAM[bus.bitfield+3]=(data & 0xFF);
            RAM[bus.bitfield+2]=(data & 0xFF00)>>8;
            RAM[bus.bitfield+1]=(data & 0xFF0000)>>16;
            RAM[bus.bitfield]=(data & 0xFF000000)>>24;
                    break;
    }
}
/*template void Bus::write(struct bus_t<uint32_t> bus, int size, uint32_t data);
template void Bus::write(struct bus_t<uint16_t> bus, int size, uint16_t data);
template void Bus::write(struct bus_t<uint8_t> bus, int size, uint8_t data);
*/
template <typename T>
    bool Bus::check_constraint(struct bus_t<T> bus, uint32_t address){
        if(address >= bus.min_range && address <= bus.max_range){
            return true;
        }
        else 
            return false;
    }
