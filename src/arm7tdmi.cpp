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

void Arm7tdmi::decode(Arm7tdmi::_instruction instruction, Bus bus_controller){
    union _instruction tmp;
    tmp.instruction = fetch(bus_controller);

    uint16_t opcode_id = 0;   
    opcode_id |= tmp.opcode_id2;
    opcode_id <<= 4;
    opcode_id |= tmp.opcode_id1;

    // 011x xxxx xxx1
    if((opcode_id >> 9 == 3) && ((opcode_id & 1) == 1))
        undefined_handler(); // undefined instruction

    switch(opcode_id)
    {
    case 0: //wip
    // and
    case 9: case 25:
    // mul
    }
}

void Arm7tdmi::execute(Arm7tdmi::_instruction instruction){
    /*Esegui il codice*/
}