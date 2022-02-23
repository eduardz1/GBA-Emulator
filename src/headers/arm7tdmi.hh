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
public: // should be made private

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


    union _instruction
    {
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

        struct
        {
            uint16_t halfword_lo;
            uint16_t halfword_hi;
        };

        uint32_t word;
    };

    enum _cond
    {
        EQ, NE, CS, CC, MI,
        PL, VS, VC, HI, LS,
        GE, LT, GT, LE, 
        AL // unconditional branch
    };

public:
    Arm7tdmi();
    ~Arm7tdmi();

    /*Fetching next instruction from the bus linked to the RAM*/
    uint32_t fetch(Bus bus_controller);

    void decode_executeARM32( _instruction instruction);
    void decode_executeTHUMB( _instruction instruction);
    void decode_execute( _instruction instruction);
    
    // set CPU mode with a change in register 15
    void set_mode(enum _mode mode);

    
    _mode get_mode();

public:
    void undef(_instruction instruction) { std::cout << "undef" << std::endl; } // ERROR
    
    // ARM instructions
    void ADC(_instruction instruction) { std::cout << "ADC" << std::endl; }
    void ADD(_instruction instruction) { std::cout << "ADD" << std::endl; }
    void AND(_instruction instruction) { std::cout << "AND" << std::endl; }
    void B(_instruction instruction, _cond condition) { std::cout << "B" << std::endl; }
    void BIC(_instruction instruction) { std::cout << "BIC" << std::endl; }
    void BX(_instruction instruction) { std::cout << "BX" << std::endl; }
    void CMN(_instruction instruction) { std::cout << "CMN" << std::endl; }
    void CMP(_instruction instruction) { std::cout << "CMP" << std::endl; }
    void EOR(_instruction instruction) { std::cout << "EOR" << std::endl; }
    void LDM(_instruction instruction) { std::cout << "LDM" << std::endl; }
    void LDR(_instruction instruction) { std::cout << "LDR" << std::endl; }
    void LDRB(_instruction instruction) { std::cout << "LDRB" << std::endl; }
    void LDRH(_instruction instruction) { std::cout << "LDRH" << std::endl; }
    void LDRSB(_instruction instruction) { std::cout << "LDRSB" << std::endl; }
    void LDRSH(_instruction instruction) { std::cout << "LDRSH" << std::endl; }
    void MLA(_instruction instruction) { std::cout << "MLA" << std::endl; }
    void MOV(_instruction instruction) { std::cout << "MOV" << std::endl; }
    void MRS(_instruction instruction) { std::cout << "MRS" << std::endl; }
    void MSR(_instruction instruction) { std::cout << "MSR" << std::endl; }
    void MUL(_instruction instruction) { std::cout << "MUL" << std::endl; }
    void MVN(_instruction instruction) { std::cout << "MVN" << std::endl; }
    void ORR(_instruction instruction) { std::cout << "ORR" << std::endl; }
    void RSB(_instruction instruction) { std::cout << "RSB" << std::endl; }
    void RSC(_instruction instruction) { std::cout << "RSC" << std::endl; }
    void SBC(_instruction instruction) { std::cout << "SBC" << std::endl; }
    void SMLAL(_instruction instruction) { std::cout << "SMLAL" << std::endl; }
    void SMULL(_instruction instruction) { std::cout << "SMULL" << std::endl; }
    void STM(_instruction instruction) { std::cout << "STM" << std::endl; }
    void STR(_instruction instruction) { std::cout << "STR" << std::endl; }
    void STRB(_instruction instruction) { std::cout << "STRB" << std::endl; }
    void STRH(_instruction instruction) { std::cout << "STRH" << std::endl; }
    void SUB(_instruction instruction) { std::cout << "SUB" << std::endl; }
    void SWI(_instruction instruction) { std::cout << "SWI" << std::endl; }
    void SWP(_instruction instruction) { std::cout << "SWP" << std::endl; }
    void SWPB(_instruction instruction) { std::cout << "SWPB" << std::endl; }
    void TEQ(_instruction instruction) { std::cout << "TEQ" << std::endl; }
    void TST(_instruction instruction) { std::cout << "TST" << std::endl; }
    void UMLAL(_instruction instruction) { std::cout << "UMLAL" << std::endl; }
    void UMULL(_instruction instruction) { std::cout << "UMULL" << std::endl; }

    // THUMB instructions
    void LSL(_instruction instruction) { std::cout << "LSL" << std::endl; }
    void LSR(_instruction instruction) { std::cout << "LSR" << std::endl; }
    void ASR(_instruction instruction) { std::cout << "ASR" << std::endl; }
    void SUBS(_instruction instruction) { std::cout << "SUBS" << std::endl; }
    void ADDS(_instruction instruction) { std::cout << "ADDS" << std::endl; }
    void ADCS(_instruction instruction) { std::cout << "ADCS" << std::endl; }
    void ANDS(_instruction instruction) { std::cout << "ANDS" << std::endl; }
    void MOVS(_instruction instruction) { std::cout << "MOVS" << std::endl; }
    void EORS(_instruction instruction) { std::cout << "EORS" << std::endl; }
    void SBCS(_instruction instruction) { std::cout << "SBCS" << std::endl; }
    void ROR(_instruction instruction) { std::cout << "ROR" << std::endl; }
    void ORRS(_instruction instruction) { std::cout << "ORRS" << std::endl; }
    void NEG(_instruction instruction) { std::cout << "NEG" << std::endl; }
    void MULS(_instruction instruction) { std::cout << "MULS" << std::endl; }
    void BICS(_instruction instruction) { std::cout << "BICS" << std::endl; }
    void MVNS(_instruction instruction) { std::cout << "MVNS" << std::endl; }
    void PUSH(_instruction instruction) { std::cout << "PUSH" << std::endl; }
    void POP(_instruction instruction) { std::cout << "POP" << std::endl; }
    void STMIA(_instruction instruction) { std::cout << "STMIA" << std::endl; }
    void LDMIA(_instruction instruction) { std::cout << "LDMIA" << std::endl; }
    void BL(_instruction instruction) { std::cout << "BL" << std::endl; }
};
}
#endif /* !ARM7TDMI_H */