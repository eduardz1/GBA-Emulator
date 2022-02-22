#include "headers/arm7tdmi.hh"

using namespace cpu;
Arm7tdmi::Arm7tdmi(){
    /*registers[CPSR].word=2882112257;
    registers[CPSR].M0=0;
    std::cout<<"After cambio"<<(uint)registers[CPSR].word<<std::endl;*/
}

Arm7tdmi::~Arm7tdmi(){ }

Arm7tdmi::_mode Arm7tdmi::get_mode()
{
    // 0x00000003 & R[15] == 0 -> ARM_MODE
    // 0x00000003 & R[15] == 2 -> THUMB_MODE
    return registers[R15].word & 0x00000003 ? THUMB_MODE : ARM_MODE;
}

void Arm7tdmi::set_mode(enum _mode mode)
{
    // in ARM state bits [1:0] of R15 are 0 and bits [31:2] contain te PC
    // in THUMB state bit [0] of R15 is 0 and bits [31:1] contain the PC
    switch (mode)
    {
    case ARM_MODE:
        registers[R15].word &= 0xFFFFFFFC;
        break;

    case THUMB_MODE:
        registers[R15].word &= 0xFFFFFFFE;
        break;
    }
}

// Passing an entire bus is too memory heavy, we need to implement it in another way
/* If fetching in THUMB_MODE, I have to read only the lower half */
uint32_t Arm7tdmi::fetch(Bus bus_controller){
        if(get_mode() == THUMB_MODE)
            return bus_controller.RAM[R15] & 0x0000FFFF;
        else 
            return bus_controller.RAM[R15];
}

void Arm7tdmi::decode_execute(Arm7tdmi::_instruction instruction){
    union _instruction tmp = instruction;

    uint8_t opcode;
    opcode = (tmp.opcode_id2 & 0x1E) >> 1; //isolating opcode bits
    

    // Undef when [25:27] -> 011 && [4] set
    if((tmp.word & 0x0E000010) == 0x06000010)
    {
        undef(instruction.word);
        return;
    }

    switch(tmp.opcode_id2 >> 6) // check bits [26:27]
    {  
    case 0x0: { 
        // Data Processing / PSR Transfer 
        if(((tmp.opcode_id2 & 0x20) == 0x20) ||
           ((tmp.opcode_id1 & 0x01) == 0) || 
           ((tmp.opcode_id1 & 0x08) == 0) )
        {
            switch(opcode)
            {
            case 0x0: AND(instruction.word); break;
            case 0x1: EOR(instruction.word); break;
            case 0x2: SUB(instruction.word); break;
            case 0x3: RSB(instruction.word); break;
            case 0x4: ADD(instruction.word); break;
            case 0x5: ADC(instruction.word); break;
            case 0x6: SBC(instruction.word); break;
            case 0x7: RSC(instruction.word); break;
            case 0x8: {
                    if (tmp.opcode_id2 & 0x1) // S[1]
                        TST(instruction.word);
                    else if ((tmp.word & 0x020F0FFF) == 0x000F0000) // I[0] && SBO [16:19] && SBZ [0:11]
                        MRS(instruction.word);
                    else
                        undef(instruction.word);
                    break;
                }
            case 0x9: {
                    if (tmp.opcode_id2 & 0x1) // S[1]
                        TEQ(instruction.word);
                    else if ((tmp.word & 0x0200F000) == 0x0200F000) // I[1] && SBO [12:15]
                        MSR(instruction.word);
                    else if ((tmp.word & 0x0200FFF0) == 0x0000F000) // I[0] && SBO [11:15] && SBZ [4:11]
                        MSR(instruction.word);
                    else if ((tmp.word & 0x020FFF10) == 0x000FFF10) // I[0] && SBO [8:19] && [4] set
                        BX(instruction.word);
                    else
                        undef(instruction.word);
                    break; 
                }
            case 0xA: {
                    if ((tmp.word & 0x0010F000) == 0x00100000) // S[1] && SBZ [12:15]
                        CMP(instruction.word);
                    else if ((tmp.word & 0x020F0FFF) == 0x000F0000) // I[0] && SBO [16:19] && SBZ [0:11]
                        MRS(instruction.word);
                    else
                        undef(instruction.word);
                    break;
                }
            case 0xB: {
                    if ((tmp.word & 0x0010F000) == 0x00100000) // S[1] && SBZ [12:15]
                        CMN(instruction.word);
                    else if ((tmp.word & 0x0200F000) == 0x0200F000) // I[1] && SBO [12:15]
                        MSR(instruction.word);
                    else if ((tmp.word & 0x0200FFF0) == 0x0000F000) // I[0] && SBO [11:15] && SBZ [4:11]
                        MSR(instruction.word);
                    else
                        undef(instruction.word);
                    break;
                }
            case 0xC: ORR(instruction.word); break;
            case 0xD: {
                    if ((tmp.word & 0xF0000) == 0x00000) // SBZ [16:19]
                        MOV(instruction.word);
                    else
                        undef(instruction.word);
                    break;
                }
            case 0xE: BIC(instruction.word); break;
            case 0xF: {
                    if ((tmp.word & 0xF0000) == 0x00000) // SBZ [16:19]
                        MVN(instruction.word);
                    else
                        undef(instruction.word);
                    break;
                }
            }
            return;
        }

        switch(tmp.opcode_id1)
        {
        case 0x9:
            switch(tmp.opcode_id2)
            {   // Multiply       
            case 0x0: case 0x1: MUL(instruction.word); break;
            case 0x2: case 0x3: MLA(instruction.word); break;
                // Multiply Long
            case 0x8: case 0x9: UMULL(instruction.word); break;
            case 0xA: case 0xB: UMLAL(instruction.word); break;
            case 0xC: case 0xD: SMULL(instruction.word); break;
            case 0xE: case 0xF: SMLAL(instruction.word); break;
                // Single Data Swap
            case 0x10: SWP(instruction.word); break;
            case 0x14: SWPB(instruction.word); break;
            }
            break;

        case 0xB:
            if((tmp.opcode_id2 & 1) == 1)
                LDRH(instruction.word);
            else
                STRH(instruction.word);
            break;

        case 0xD:
            if((tmp.opcode_id2 & 1) == 1)
                LDRSB(instruction.word);
            else
                undef(instruction.word);
            break;

        case 0xF:
            if((tmp.opcode_id2 & 1) == 1)
                LDRSH(instruction.word);
            else
                undef(instruction.word);
            break;
        }
        break;
    }

    case 0x1: {
        switch(tmp.opcode_id2 & 0x5) // isolate bits 20 and 22
        {
        case 0x0: STR(instruction.word); break;  // xxxx x0x0
        case 0x1: LDR(instruction.word); break;  // xxxx x0x1
        case 0x4: STRB(instruction.word); break; // xxxx x1x0
        case 0x5: LDRB(instruction.word); break; // xxxx x1x1
        }
        break;
    }

    case 0x2: {
        switch(tmp.opcode_id2 & 0x21) // isolate bits 20 and 25
        {
        case 0x20: case 0x21: B(instruction.word); break; // xx1x xxx0 U xx1x xxx1
        case 0x01: LDM(instruction.word); break; // xx0x xxx1
        case 0x00: STM(instruction.word); break; // xx0x xxx0
        }
        break;
    }

    case 0x3: {   
        if(tmp.opcode_id2 & 0x30)
            SWI(instruction.word);
        else 
            undef(instruction.word);
        break;
    }
    }
}