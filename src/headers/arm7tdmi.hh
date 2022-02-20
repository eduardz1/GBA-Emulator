#ifndef ARM7TDMI_H
#define ARM7TDMI_H
#include <array>
#include <stdio.h>
#include <iostream>
#include "bus.hh"
namespace cpu
{ 
class Arm7tdmi
{
private:

    enum _mode
    {
        ARM_MODE,
        THUMB_MODE
    };

    enum _registers
    {
        R0, R1, R2, R3, R4, R5, R6, R7, R8, R9,R10,R11,R12,//General Purpose Registers
        R13,//Stack Pointer Register
        R14,//Link Register
        R15,//Program Counter
        R8_fiq,R9_fiq, R10_fiq,R11_fiq,R12_fiq, //Registers with special mode
        R13_fiq, R13_svc, R13_abt, R13_irq, R13_und,//Registers with special mode
        R14_fiq, R14_svc, R14_abt, R14_irq, R14_und, //Registers with special mode
        CPSR, SPSR_fiq, SPSR_svc, SPSR_abt, SPSR_irq, SPSR_und,//Program Status Registers
        
        NUM // Number of Registers
    };

    //By using the union, we can access single bits fro the uint32_t word parameter by switching to the single bit(i.e. bit)
    union _register
    { 
        struct{
            uint32_t M0:1;//Mode bit 
            uint32_t M1:1;//Mode bit
            uint32_t M2:1;//Mode bit
            uint32_t M3:1;//Mode bit
            uint32_t M4:1;//Mode bit
            uint32_t T:1;//State bit 
            uint32_t F:1;//FIQ Disable 
            uint32_t I:1;//IRQ Disable 
            uint32_t R:20;//RESERVED BITS, NOT USEFUL
            uint32_t V:1;//Overflow FLAG
            uint32_t C:1;//Carry/Borrow/Extended FLAG
            uint32_t Z:1;//Zero FLAG
            uint32_t N:1;//Negative/Less than FLAG
        };
        uint32_t word;
    };

    std::array<_register,NUM>registers;

    enum _instruction_type{
        DATA_PROCESSING,
        PSR_TRANSFER,
        MULTIPLY,
        MULTIPLY_LONG,
        SINGLE_DATA_SWAP,
        BRANCH_EXCHANGE,
        HDT_REG_OFFSET,//Halfword Data Transfer
        HDT_IMM_OFFSET,//Halfword Data Transfer
        SINGLE_DATA_TRANSFER,
        UNDEFINED,
        BLOCK_DATA_TRANSFER,
        BRANCH,
        COPROCESSOR_DATA_TRANSFER,
        COPROCESSOR_DATA_OPERATION,
        COPROCESSOR_REG_TRANSFER,
        SOFTWARE_INTERRUPT
    };

    /*bit-mask for every type of instruction. They will be used to decode instructions*/
    struct _instruction_masks{
    };


    struct _instruction{
        enum _instruction_type type;
        uint32_t instruction;
    };

public:
    Arm7tdmi();
    ~Arm7tdmi();

    /*Fetching next instruction from the bus linked to the RAM*/
    uint32_t fetch(Bus bus_controller);
    /*Decoding the instruction to be executed*/
    _instruction decode( _instruction instruction);
    /*Executing the instruction*/
    void execute(_instruction instruction);
    // set CPU mode with a change in register 15
    void set_mode(enum _mode mode);

    // when would we use get_mode if not for debugging? I think that it would
    // make sense for it to return the string with the mode
    std::string get_mode();


};
}
#endif /* !ARM7TDMI_H */