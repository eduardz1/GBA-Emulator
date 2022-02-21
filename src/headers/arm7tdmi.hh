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
        struct
        {
            uint32_t M0 : 1; // Mode bit
            uint32_t M1 : 1; // Mode bit
            uint32_t M2 : 1; // Mode bit
            uint32_t M3 : 1; // Mode bit
            uint32_t M4 : 1; // Mode bit
            uint32_t T : 1;  // State bit
            uint32_t F : 1;  // FIQ Disable
            uint32_t I : 1;  // IRQ Disable
            uint32_t R : 20; // RESERVED BITS, NOT USEFUL
            uint32_t V : 1;  // Overflow FLAG
            uint32_t C : 1;  // Carry/Borrow/Extended FLAG
            uint32_t Z : 1;  // Zero FLAG
            uint32_t N : 1;  // Negative/Less than FLAG
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


    union _instruction{
        // 
        struct
        {
            uint32_t Rm : 4;
            uint32_t opcode_id1 : 4;
            uint32_t Rs : 4;
            uint32_t Rd : 4;
            uint32_t Rn : 4;
            uint32_t opcode_id2 : 8;
            uint32_t cond : 4;
        };
        uint32_t instruction;
    };

public:
    Arm7tdmi();
    ~Arm7tdmi();

    /*Fetching next instruction from the bus linked to the RAM*/
    uint32_t fetch(Bus bus_controller);
    /*Decoding the instruction to be executed*/
    void decode( _instruction instruction, Bus bus_controller);
    /*Executing the instruction*/
    void execute(_instruction instruction);
    // set CPU mode with a change in register 15
    void set_mode(enum _mode mode);

    // when would we use get_mode if not for debugging? I think that it would
    // make sense for it to return the string with the mode
    std::string get_mode();

public:
    void undefined_handler(); // ERROR
    void ADC(uint32_t instruction);
    void ADD(uint32_t instruction);
    void AND(uint32_t instruction);
    void B(uint32_t instruction);
    void BIC(uint32_t instruction);
    void BX(uint32_t instruction);
    void CMN(uint32_t instruction);
    void CMP(uint32_t instruction);
    void EOR(uint32_t instruction);
    void LDM(uint32_t instruction);
    void LDR(uint32_t instruction);
    void LDRB(uint32_t instruction);
    void LDRH(uint32_t instruction);
    void LDRSB(uint32_t instruction);
    void LDRSH(uint32_t instruction);
    void MLA(uint32_t instruction);
    void MOV(uint32_t instruction);
    void MRS(uint32_t instruction);
    void MSR(uint32_t instruction);
    void MUL(uint32_t instruction);
    void MVN(uint32_t instruction);
    void ORR(uint32_t instruction);
    void RSB(uint32_t instruction);
    void RSC(uint32_t instruction);
    void SBC(uint32_t instruction);
    void SMLAL(uint32_t instruction);
    void SMULL(uint32_t instruction);
    void STM(uint32_t instruction);
    void STR(uint32_t instruction);
    void STRB(uint32_t instruction);
    void STRH(uint32_t instruction);
    void SUB(uint32_t instruction);
    void SWI(uint32_t instruction);
    void SWP(uint32_t instruction);
    void SWPB(uint32_t instruction);
    void TEQ(uint32_t instruction);
    void TST(uint32_t instruction);
    void UMLAL(uint32_t instruction);
    void UMULL(uint32_t instruction);
};
}
#endif /* !ARM7TDMI_H */