#include "headers/arm7tdmi.hh"
using namespace cpu;
// Rd = Rd + Rs + C-bit (THUMB)

/**
 * @brief Shift Rs left by a 5-bit immediate 
 * value and store the result in Rd.
 */
void Arm7tdmi::LSL_t(Arm7tdmi::_instruction ins)
{
    uint8_t val; // Immediate Value
    _register_type Rd; // Destination Register
    _register_type Rs; // Source Register

    Rd = registers[(_registers)(ins.halfword_lo & 0x7)];
    Rs = registers[(_registers)((ins.halfword_lo >> 3) & 0x7)];
    set_register((_registers)Rd.word, (Rs.word << (val)));
}

/**
 * @brief Perform logical shift right on Rs by a 5-bit 
 * immediate value and store the result in Rd.
 */
void Arm7tdmi::LSR_t(Arm7tdmi::_instruction ins)
{
    uint8_t val; // Immediate Value
    _register_type Rd; // Destination Register
    _register_type Rs; // Source Register

    Rd = registers[(_registers)(ins.halfword_lo & 0x7)];
    Rs = registers[(_registers)((ins.halfword_lo >> 3) & 0x7)];
    set_register((_registers)Rd.word, (Rs.word << (val)));
}

void Arm7tdmi::NEG_t(Arm7tdmi::_instruction instruction)
{
    _register_type rd, rn;
    rd = registers[(_registers)(instruction.halfword_lo & 0xF)];
    rn = registers[(_registers)((instruction.halfword_lo >> 4) & 0xF)]; // rn is rs in THUMB
    rd.word = 0 - rn.word;
}

/**
 * @brief Perform arithmetic shift right on Rs by a 5-bit
 *  immediate value and store the result in Rd.
 */
void Arm7tdmi::ASR_t(Arm7tdmi::_instruction ins)
{
    uint8_t sign; // Most significant bit of Rs
    uint8_t val; // Immediate Value
    _register_type Rd; // Destination Register
    _register_type Rs; // Source Register

    Rd = registers[(_registers)(ins.halfword_lo & 0x7)];
    Rs = registers[(_registers)((ins.halfword_lo >> 3) & 0x7)];
    sign = Rs.word & 0x80000000;
    set_register((_registers)Rd.word, ((Rs.word >> (val)) | sign));
}

/**
 * @brief if I[0]: Subtract contents of Rn to contents of Rs. Place result in Rd.
 * else: Subtract 3-bit immediate value to contents of Rs. Place result in Rd.
 */
void Arm7tdmi::SUB_t(Arm7tdmi::_instruction ins)
{
    _register_type Rd; // Destination Register
    _register_type Rs; // Source Register
    int32_t Rn_Offset3; // Register/Immediate Value

    Rd = registers[(_registers)(ins.halfword_lo & 0x7)];
    Rs = registers[(_registers)((ins.halfword_lo >> 3) & 0x7)];

    if(ins.halfword_lo & 0x0400) // Check flag I
        Rn_Offset3 = ((ins.halfword_lo >> 6) & 0x7);
    else
        registers[(_registers)((ins.halfword_lo >> 6) & 0x7)].word;
            
    set_register((_registers)Rd.word, (Rs.word - Rn_Offset3));
}

void Arm7tdmi::ADC_t(Arm7tdmi::_instruction instruction)
{
    _register_type rd,rn,op2;
    rd=registers[(_registers)(instruction.halfword_lo&0xF)];
    rn=registers[(_registers)((instruction.halfword_lo>>4)&0xF)];//Rn is Rs in THUMB
    rd.word=(rd.word+rn.word)+registers[CPSR].C;//Not sure if cpu should perform ALU operations in 32 bit
}

/**
 * @brief if I[0]: Add contents of Rn to contents of Rs. Place result in Rd.
 * else: Add 3-bit immediate value to contents of Rs. Place result in Rd.
 */
void Arm7tdmi::ADD_t(Arm7tdmi::_instruction ins) // NEED TO IMPLEMENT LATER IN A MEGA ADD
{
    _register_type Rd; // Destination Register
    _register_type Rs; // Source Register
    int32_t Rn_Offset3; // Register/Immediate Value

    if(ins.halfword_lo )
    Rd = registers[(_registers)(ins.halfword_lo & 0x7)];
    Rs = registers[(_registers)((ins.halfword_lo >> 3) & 0x7)];

    if(ins.halfword_lo & 0x0400) // Check flag I
        Rn_Offset3 = ((ins.halfword_lo >> 6) & 0x7);
    else
        Rn_Offset3 = registers[(_registers)((ins.halfword_lo >> 6) & 0x7)].word;
            
    //set_register((_registers)Rd.word, (Rs.word + Rn_Offset3));
    Rd.word = (Rs.word + Rn_Offset3);
}

/**
 * @brief 
 */
void Arm7tdmi::AND_t(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=registers[(_registers)(instruction.Rd)];
    rn=registers[(_registers)(instruction.Rn)];
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/
}
// void Arm7tdmi::MOVS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::EOR_a(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=registers[(_registers)(instruction.halfword_lo&0xF)];
    rn=registers[(_registers)((instruction.halfword_lo>>4)&0xF)];//rn is rs in THUMB
    rd.word=rd.word^rn.word;
}
// void Arm7tdmi::SBCS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::ROR_t(Arm7tdmi::_instruction instruction)
{
    // https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
    _register_type rd, rn;
    rd = registers[(_registers)(instruction.halfword_lo & 0xF)];
    rn = registers[(_registers)((instruction.halfword_lo >> 4) & 0xF)]; // rn is rs in THUMB
    rd.word = (rd.word >> rn.word) | (rd.word << (32 - rn.word));
}
// void Arm7tdmi::ORRS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::MUL_t(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=registers[(_registers)(instruction.halfword_lo&0xF)];
    rn=registers[(_registers)((instruction.halfword_lo>>4)&0xF)];//rn is rs in THUMB
    rd.word=rd.word*rn.word;
    }
void Arm7tdmi::BIC_t(Arm7tdmi::_instruction instruction){
     _register_type rd,rn,op2;
    rd=registers[(_registers)(instruction.halfword_lo&0xF)];
    rn=registers[(_registers)((instruction.halfword_lo>>4)&0xF)];//rn is rs in THUMB
    rd.word=rd.word&~rn.word;
}
// void Arm7tdmi::MVNS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::PUSH_t(Arm7tdmi::_instruction instruction) {}
void Arm7tdmi::POP_t(Arm7tdmi::_instruction instruction) {}
// void Arm7tdmi::STMIA(Arm7tdmi::_instruction instruction){}
// void Arm7tdmi::LDMIA(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::BL_t(Arm7tdmi::_instruction instruction) {}