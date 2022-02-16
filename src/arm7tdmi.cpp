#include "headers/arm7tdmi.hh"

std::string arm7tdmi::get_mode()
{
    // 0x00000003 & R[15] == 0 -> ARM_MODE
    // 0x00000003 & R[15] == 2 -> THUMB_MODE
    return registers[R15] & 0x00000003 ? "THUMB_MODE" : "ARM_MODE";
}

void arm7tdmi::set_mode(enum _mode mode)
{
    // in ARM state bits [1:0] of R15 are 0 and bits [31:2] contain te PC
    // in THUMB state bit [0] of R15 is 0 and bits [31:1] contain the PC
    switch (mode)
    {
    case ARM_MODE:
        registers[R15] &= 0xFFFFFFFC;
        break;

    case THUMB_MODE:
        registers[R15] &= 0xFFFFFFFE;
        break;

    default:
        // ERROR
        break;
    }
}