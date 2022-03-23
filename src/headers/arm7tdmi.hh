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

    enum _mode
    {
        ARM_MODE,
        THUMB_MODE
    };

    enum _registers
    {
        R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12,//General Purpose Registers
        R13,//Stack Pointer Register
        R14,//Link Register
        R15,//Program Counter
        R8_fiq, R9_fiq, R10_fiq, R11_fiq, R12_fiq, //Registers with special mode
        R13_fiq, R13_svc, R13_abt, R13_irq, R13_und,//Registers with special mode
        R14_fiq, R14_svc, R14_abt, R14_irq, R14_und, //Registers with special mode
        CPSR, SPSR_fiq, SPSR_svc, SPSR_abt, SPSR_irq, SPSR_und,//Program Status Registers
        
        NUM // Number of Registers
    };

    enum _access_mode{
        USR,
        FIQ,
        IRQ,
        SVC,
        res1,res2,res3,//Reserved. Used only for enum purposes
        ABT,
        res4,res5,res6,//Reserved. Used only for enum purposes
        UND,
        res7,res8,res9,//Reserved. Used only for enum purposes
        SYS
    };
    
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

    enum _shift{
        LL,//logical left
        LR,//logical right  shifted bits are replaced by 0 without taking into account the MSB bit(aka sign bit)
        AR,//arithmetic right  shifted bits are replaced by 0 if positive, 1 otherwise
        RR//rotate right
    };
    enum _cond
    {
        EQ, NE, CS, CC, MI,
        PL, VS, VC, HI, LS,
        GE, LT, GT, LE, 
        AL, // unconditional branch
        RESERVED//THUMB only, it causes the branch to behave like a SWI instruction
    };

    // Rd usually is the destination register
    // Rn usually is the 1st operand register
    std::array<_register_type,NUM>registers;
    uint8_t ACCESS_MODE=USR;

public:
    Arm7tdmi();
    ~Arm7tdmi();

    /*Fetching next instruction from the bus linked to the RAM*/
    uint32_t fetch(Bus bus_controller);

    void decode_executeARM32( _instruction instruction);
    void decode_executeTHUMB( _instruction instruction);
    void decode_execute( _instruction instruction);

    int32_t read_from_memory(Bus bus_controller,uint32_t address);
    void write_to_memory(Bus bus_controller,int32_t value,uint32_t address);
    
    _access_mode get_access_mode();
    void set_access_mode(_access_mode mode);

    
    _mode get_mode();
    void set_mode(enum _mode mode);// set CPU mode with a change in register 15

    void set_register(_registers reg, uint32_t val);

    bool evaluate_cond(_cond condition);
    _registers get_register(_registers id);
    int32_t get_ALU_op2(_shift type, _instruction ins);
    void set_condition_code_flags(int32_t Rd, int32_t Rn, int32_t op2, bool overflowable);

public: // ARM32 & THUMB ISA
    void undef(_instruction instruction) { std::cout << "undef" << std::endl; } // ERROR
    void exception_handler();
    
    #pragma region // ARM instruction set
    void ADC_a(_instruction);           void MUL_a(_instruction);
    void ADD_a(_instruction);           void SMLAL_a(_instruction);
    void AND_a(_instruction);           void SMULL_a(_instruction);
    void B_a(_instruction);             void UMLAL_a(_instruction); 
    void BIC_a(_instruction);           void UMULL_a(_instruction);
    void BL_a(_instruction);    
    void BX_a(_instruction);            void MVN_a(_instruction);
    /* void CDP_a(_instruction); */     void ORR_a(_instruction);
    void CMN_a(_instruction);           void RSB_a(_instruction);
    void CMP_a(_instruction);           void RSC_a(_instruction);
    void EOR_a(_instruction);           void SBC_a(_instruction);
    /* void LDC_a(_instruction); */     /* void STC_a(_instruction); */
    void LDM_a(_instruction);           void STM_a(_instruction); 

    void LDR_a(_instruction);           void STR_a(_instruction);
    void LDRB_a(_instruction);          void STRB_a(_instruction);
    void LDRH_a(_instruction);          void STRH_a(_instruction);
    void LDRSB_a(_instruction);         
    void LDRSH_a(_instruction);         void SUB_a(_instruction);
                                        void SWI_a(_instruction);
    /* void MCR_a(_instruction); */     void SWP_a(_instruction);
    void MLA_a(_instruction);           void SWPB_a(_instruction);
    void MOV_a(_instruction);   
    /* void MRC_a(_instruction); */      
    void MRS_a(_instruction);           void TEQ_a(_instruction);
    void MSR_a(_instruction);           void TST_a(_instruction);
    #pragma endregion
    
    #pragma region // THUMB instruction set
    void ADC_t(_instruction);           void LDSB_t(_instruction); 
    void ADD_t(_instruction);           void LDSH_t(_instruction);
    void AND_t(_instruction);           
    void ASR_t(_instruction);           void LSR_t(_instruction);
    void B_t(_instruction, _cond);      void MOV_t(_instruction);
    void Bxx_t(_instruction); /*(?)*/   void MUL_t(_instruction);
    void BIC_t(_instruction);           void MVN_t(_instruction);
    void BL_t(_instruction);            void NEG_t(_instruction);
    void BX_t(_instruction);            void ORR_t(_instruction);
    void CMN_t(_instruction);           void POP_t(_instruction);
    void CMP_t(_instruction);           void PUSH_t(_instruction);
    void EOR_t(_instruction);           void ROR_t(_instruction);
    void LDMIA_t(_instruction);         void SBC_t(_instruction);
                                        void STMIA_t(_instruction); 
    void LDR_t(_instruction);            
    void LDRB_t(_instruction);          void STR_t(_instruction);
    void LDRH_t(_instruction);          void STRB_t(_instruction);
    void LDRSB_t(_instruction);         void STRH_t(_instruction);
    void LDRSH_t(_instruction);         
                                        void SWI_t(_instruction);
                                        void SUB_t(_instruction);
    void LSL_t(_instruction);           void TST_t(_instruction);
    #pragma endregion
};
}
#endif /* !ARM7TDMI_H */