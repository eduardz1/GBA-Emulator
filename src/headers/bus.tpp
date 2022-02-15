#pragma once
template <typename T>
T Bus::read(struct bus_t<T> bus, int size)
{
    printf("Bus.bitfield is %d\n", bus.bitfield);
    printf("Size is %d\n", size);
    return bus.bitfield >> size; // 10 0001 = 33
}

template uint32_t Bus::read(struct bus_t<uint32_t> bus, int size);

template <typename T>
void Bus::write(struct bus_t<T> bus, int size, T data)
{
    //read<uint32_t>(bus, size);
    bus.bitfield = data;
}