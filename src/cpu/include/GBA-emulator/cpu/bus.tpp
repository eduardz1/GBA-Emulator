#pragma once

template <typename T> T Bus::read(struct _bus<T> bus, int bit_length)
{
    T tmp = 0;
    // 18 00 00 EA
    switch (bit_length)
    {
    case 8: // reading 1 byte
        tmp = RAM[bus.bitfield];
        break;

    case 16: // reading 2 bytes in LITTLE ENDIAN
        tmp = RAM[bus.bitfield];
        tmp |= RAM[bus.bitfield + 1] << 8;
        break;

    case 32: // reading 4 bytes in LITTLE ENDIAN
        tmp = RAM[bus.bitfield];
        tmp |= RAM[bus.bitfield + 1] << 8;
        tmp |= RAM[bus.bitfield + 2] << 16;
        tmp |= RAM[bus.bitfield + 3] << 24;
        break;

    default:
        // ERROR
        break;
    }

    return tmp;
}

template <typename T>
void Bus::write(struct _bus<T> bus, int bit_length, T data)
{
    switch (bit_length)
    {
    case 8: // writing 1 byte
        RAM[bus.bitfield] = data;
        break;

    case 16: // writing 2 bytes
        RAM[bus.bitfield + 1] = (data & 0x00FF);
        RAM[bus.bitfield] = (data & 0xFF00) >> 8;
        break;

    case 32: // writing 4 bytes
        std::cout << "Dato da inserire:" << data << std::endl;

        RAM[bus.bitfield + 3] = (data & 0x000000FF);
        RAM[bus.bitfield + 2] = (data & 0x0000FF00) >> 8;
        RAM[bus.bitfield + 1] = (data & 0x00FF0000) >> 16;
        RAM[bus.bitfield] = (data & 0xFF000000) >> 24;
        break;

    default:
        // ERROR
        break;
    }
}

template <typename T>
bool Bus::check_constraint(struct _bus<T> bus, uint32_t address)
{
    if (address >= bus.min_range && address <= bus.max_range)
        return true;
    else
        return false;
}