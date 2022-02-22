#include "headers/arm7tdmi.hh"

using namespace cpu;
Arm7tdmi::Arm7tdmi(){
    registers[CPSR].word=2882112257;
   /* printf("M0:%u\n",registers[CPSR].M0 & 0x1);
    printf("M1:%u\n",registers[CPSR].M1 & 0x1);
    printf("M2:%u\n",registers[CPSR].M2 & 0x1);
    printf("M3:%u\n",registers[CPSR].M3 & 0x1);
    printf("N:%u\n",registers[CPSR].N &0x1);*/
    registers[CPSR].M0=0;
    std::cout<<"After cambio"<<(uint)registers[CPSR].word<<std::endl;
}
 Arm7tdmi::~Arm7tdmi(){
 }

std::string Arm7tdmi::get_mode()
{
    // 0x00000003 & R[15] == 0 -> ARM_MODE
    // 0x00000003 & R[15] == 2 -> THUMB_MODE
    return registers[R15].word & 0x00000003 ? "THUMB_MODE" : "ARM_MODE";
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

    default:
        // ERROR
        break;
    }
}

/* If fetching in THUMB_MODE, I have to read only the lower half */
uint32_t Arm7tdmi::fetch(Bus bus_controller){
        if(get_mode().compare("THUMB_MODE") == 0)
            return bus_controller.RAM[R15] & 0x0000FFFF;
        else 
            return bus_controller.RAM[R15];
}

void Arm7tdmi::decode_execute(Arm7tdmi::_instruction instruction){
    union _instruction tmp = instruction;
    uint8_t opcode;
    // tmp.word = //fetch(bus_controller);

    uint16_t opcode_id = 0;   
    opcode_id |= tmp.opcode_id2;
    opcode_id <<= 4;
    opcode_id |= tmp.opcode_id1;

    opcode=(tmp.opcode_id2 & 30)>>1;//isolating opcode bits

    // 011x xxxx xxx1
    if((opcode_id >> 9 == 3) && ((opcode_id & 1) == 1))
        undefined_handler(); // undefined instruction

    switch(tmp.opcode_id2>>6)
    {  
    case 0x0: // bit 26-27 unset
        {
        if(tmp.opcode_id2 & 32 == 0 || !(tmp.opcode_id1 & 1 == 0) || !(tmp.opcode_id1 & 8 == 0)){//bit 25 set or bit 4 unset  or bit 7 unset
            switch(opcode){
                case 0x0:AND(instruction.word); break;
                case 0x1:EOR(instruction.word); break;
                case 0x2:SUB(instruction.word); break;
                case 0x3:RSB(instruction.word); break;
                case 0x4:ADD(instruction.word); break;
                case 0x5:ADC(instruction.word); break;
                case 0x6:SBC(instruction.word); break;
                case 0x7:RSC(instruction.word); break;
                case 0x8:{
                    if(tmp.opcode_id2&1){//bit 20 set
                        TST(instruction.word); 
                    }else{
                        if((tmp.word>>16)&15 == 1 && ((tmp.word>>12)==0) && (tmp.opcode_id2&32)==0 ){//bit[16:19] set and bit[0:11] unset and bit[25] unset
                            MRS(instruction.word);
                        }else{
                            undefined_handler();
                        }
                    }
                    break;
                }
                case 0x9:{
                    if(tmp.opcode_id2 & 1){// bit 20 set(S flag == 1)
                        TEQ(instruction.word);
                    }else{//bit 20 unset(S flag == 0)
                        if(tmp.opcode_id2 & 32 == 0){//bit 25 set(I flag == 1)
                            if((tmp.word>>12)&15==15){//bit[12:15] set
                                MSR(instruction.word);
                            }else 
                                undefined_handler();
                        }else{//bit 25 unset(I flag == 0)
                            if(tmp.opcode_id1 == 0){//bit[4:7] unset
                                if((tmp.word >>12)&15 == 15 && (tmp.word >>12)&15 == 0){//bit[12:15] set and bit[8:11] unset
                                    MSR(instruction.word);
                                }else
                                    undefined_handler();
                            }else if(tmp.opcode_id1 == 1){
                                    if((tmp.word>>8)&4095 == 4095){//bit[8:19] set
                                        BX(instruction.word);
                                    }else
                                        undefined_handler();
                            }else 
                                undefined_handler();
                        }
                    }
                    break;
                }
                case 0xA:{
                    if((tmp.opcode_id2&1)==1){//bit 20 set(S flag == 1)
                        if((tmp.word>>12)&15 == 0){//bit[12:15] unset
                            CMP(instruction.word);
                        }else{
                            undefined_handler();
                        }
                    }else{//bit 20 unset(S flag == 0)
                        if((tmp.word>>16)&15==1 && (tmp.word&4095) == 0 && (tmp.opcode_id2&32)==0){//bit[16:19] set and bit[0:11] unset and bit[25] unset
                            MRS(instruction.word);
                        }else 
                            undefined_handler();
                    }
                   
                    break;
                }
                case 0xB:{
                    if((tmp.opcode_id2&1)==1){//bit 20 set(S flag == 0)
                        if(((tmp.word>>12) & 15) == 0){//bit[12:15] unset
                            CMN(instruction.word);
                        }else{
                            undefined_handler();
                        }
                    }else{//bit 20 unset(S flag == 1)
                        if((tmp.opcode_id2&32) == 1){//bit 25 set(I flag == 1)
                            if(((tmp.word>>12) & 15) == 1){
                                MSR(instruction.word);
                            }else{
                                undefined_handler();
                            }
                        }else{//bit 25 unset (I flag == 0)
                            if(tmp.opcode_id1 == 0 && ((tmp.word>>12)&15 ==1) && (tmp.word>>8)&15 == 0){//bit [4:7] unset and bit[12:15] set and bit[8:11] unset
                                MSR(instruction.word);
                            }else{
                                undefined_handler();
                            }
                        }
                    }
                    break;
                
                }
                case 0xC:ORR(instruction.word); break;
                case 0xD:{
                    if(((tmp.word>>16) & 15)==0){//bit[16:19] unset
                        MOV(instruction.word);
                    }else{
                        undefined_handler();
                    }
                    break;
                }
                case 0xE:BIC(instruction.word); break;
                case 0xF:{
                     if(((tmp.word>>16) & 15)==0){//bit[16:19] unset
                        MVN(instruction.word);
                    }else{
                        undefined_handler();
                    }
                    break;
                }
            }
            return;
        }

        switch(tmp.opcode_id1)
        {
        case 0x9:
            switch(tmp.opcode_id2)
            {   
                // Multiply
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

                default:
                // Halfword Data Transfer: register/immediate
                // Single Data Transfer
                // Block Data Transfer
                // Branch
                // Coprocessor Data Transfer
                // Coprocessor Register Transfer
                // Software Interrupt
                break;
            }
            break;

        case 0xB:
            if(tmp.opcode_id2 & 1 == 1)
                LDRH(instruction.word);
            else
                STRH(instruction.word);
            break;

        case 0xD:
            if(tmp.opcode_id2 & 1 == 1)
                LDRSB(instruction.word);
            else
                undefined_handler();
            break;

        case 0xF:
            if(tmp.opcode_id2 & 1 == 1)
                LDRSH(instruction.word);
            else
                undefined_handler();
            break;
        }
        }
        break;

    case 0x1:
        {
            if(tmp.opcode_id2 & 1 == 1)
            {
                if(tmp.opcode_id2 & 4 == 1) LDRB(instruction.word);
                else LDR(instruction.word);
            }
            else
            {
                if(tmp.opcode_id2 & 4 == 1) STRB(instruction.word);
                else STR(instruction.word);
            }

        }
        break;

    case 0x2:
        {   
            if(tmp.opcode_id2 & 0x20 == 1)
            {
                B(instruction.word);
            }
            else
            {
                if(tmp.opcode_id2 & 1 == 1) LDM(instruction.word);
                else STM(instruction.word);
            }
        }
        break;

    case 0x3:
        {   
            if(tmp.opcode_id2 & 0x30) SWI(instruction.word);
            else undefined_handler();
        }
        break;
    }
}
