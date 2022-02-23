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
    union _register_type
    {
        struct
        {
            int32_t M0 : 1; // Mode bit
            int32_t M1 : 1; // Mode bit
            int32_t M2 : 1; // Mode bit
            int32_t M3 : 1; // Mode bit
            int32_t M4 : 1; // Mode bit
            int32_t T : 1;  // State bit
            int32_t F : 1;  // FIQ Disable
            int32_t I : 1;  // IRQ Disable
            int32_t R : 20; // RESERVED BITS, NOT USEFUL
            int32_t V : 1;  // Overflow FLAG
            int32_t C : 1;  // Carry/Borrow/Extended FLAG
            int32_t Z : 1;  // Zero FLAG
            int32_t N : 1;  // Negative/Less than FLAG
        };
        int32_t word;
    };
    /*Rd usually is the destination register
      Rn usually is the 1st operand register*/
    std::array<_register_type,NUM>registers;

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

    enum _shift{
        LL,//logical left
        LR,//logical right
        AR,//arithmetic right
        RR//rotate riht
    };
    enum _cond
    {
        EQ, NE, CS, CC, MI,
        PL, VS, VC, HI, LS,
        GE, LT, GT, LE, 
        AL, // unconditional branch
        RESERVED//THUMB only, it causes the branch to behave like a SWI instruction
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
    _cond get_cond(_instruction instruction);
    _register_type get_register(_registers reg);
    void set_register(_registers reg, uint32_t val);

public:
    void undef(_instruction instruction) { std::cout << "undef" << std::endl; } // ERROR
    
    // ARM instructions
    void ADD(_instruction instruction);
    void ADC(_instruction instruction);
    void AND(_instruction instruction);
    void B(_instruction instruction, _cond condition);
    void BIC(_instruction instruction);
    void BX(_instruction instruction);
    void CMN(_instruction instruction);
    void CMP(_instruction instruction);
    void EOR(_instruction instruction);
    void LDM(_instruction instruction);
    void LDR(_instruction instruction);
    void LDRB(_instruction instruction);
    void LDRH(_instruction instruction);
    void LDRSB(_instruction instruction);
    void LDRSH(_instruction instruction);
    void MLA(_instruction instruction);
    void MOV(_instruction instruction);
    void MRS(_instruction instruction);
    void MSR(_instruction instruction);
    void MUL(_instruction instruction);
    void MVN(_instruction instruction);
    void ORR(_instruction instruction);
    void RSB(_instruction instruction);
    void RSC(_instruction instruction);
    void SBC(_instruction instruction);
    void SMLAL(_instruction instruction);
    void SMULL(_instruction instruction);
    void STM(_instruction instruction); 
    void STR(_instruction instruction); 
    void STRB(_instruction instruction);
    void STRH(_instruction instruction);
    void SUB(_instruction instruction);
    void SWI(_instruction instruction);
    void SWP(_instruction instruction);
    void SWPB(_instruction instruction);
    void TEQ(_instruction instruction); 
    void TST(_instruction instruction); 
    void UMLAL(_instruction instruction); 
    void UMULL(_instruction instruction); 

    // THUMB instructions
    void LSL(_instruction instruction); 
    void LSR(_instruction instruction);
    void ASR(_instruction instruction);
    void SUBS(_instruction instruction);
    void ADDS(_instruction instruction);
    void ADCS(_instruction instruction);
    void ANDS(_instruction instruction);
    void MOVS(_instruction instruction);
    void EORS(_instruction instruction);
    void SBCS(_instruction instruction);
    void ROR(_instruction instruction); 
    void ORRS(_instruction instruction);
    void NEG(_instruction instruction);
    void MULS(_instruction instruction);
    void BICS(_instruction instruction);
    void MVNS(_instruction instruction);
    void PUSH(_instruction instruction);
    void POP(_instruction instruction);
    void STMIA(_instruction instruction); 
    void LDMIA(_instruction instruction);
    void BL(_instruction instruction);
};
}
#endif /* !ARM7TDMI_H */