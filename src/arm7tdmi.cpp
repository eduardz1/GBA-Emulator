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

void Arm7tdmi::decode_execute(Arm7tdmi::_instruction instruction, Bus bus_controller){
    union _instruction tmp;
    tmp.word = fetch(bus_controller);

    uint16_t opcode_id = 0;   
    opcode_id |= tmp.opcode_id2;
    opcode_id <<= 4;
    opcode_id |= tmp.opcode_id1;

    // 011x xxxx xxx1
    if((opcode_id >> 9 == 3) && ((opcode_id & 1) == 1))
        undefined_handler(); // undefined instruction

    switch(tmp.opcode_id1)
    {
    case 0x1:
        switch(tmp.opcode_id2)
        {
            case 0x12: BX(instruction.word); break;
        }

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
        }
    }
}
