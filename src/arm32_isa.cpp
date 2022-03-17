#include "headers/arm7tdmi.hh"
using namespace cpu;
// TODO: check for carry flag
// FIXME: implement "ADD" style code in the other ALU functions
#pragma region // ALU OPERATIONS
void Arm7tdmi::ADD(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word + op2;
    set_register((_registers)(ins.Rd),Rd);
    set_condition_code_flags(Rd, Rn.word, op2);
}

/* Rd = Rn + Op2+ C-bit (ARM32) */
void Arm7tdmi::ADC(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }

    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word + (op2 << shift_amount) + registers[get_register(CPSR)].C;
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word + ((uint32_t)op2 >> shift_amount) + registers[get_register(CPSR)].C;
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word + (op2 >> shift_amount) + registers[get_register(CPSR)].C;
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word + ((op2 >> shift_amount) | (op2 << (32 - shift_amount))) + registers[get_register(CPSR)].C;
        break;
    }
    */
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word + op2+registers[get_register(CPSR)].C;
    set_register((_registers)(ins.Rd),Rd);
    set_condition_code_flags(Rd, Rn.word, op2);
}
void Arm7tdmi::SUBS(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word - (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word - ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word - (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word - ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word - op2;
    set_register((_registers)(ins.Rd),Rd);
    set_condition_code_flags(Rd, Rn.word, op2);
}
void Arm7tdmi::AND(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word & (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }*/
    if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type,ins);
    Rd = Rn.word & op2;
    set_register((_registers)(ins.Rd),Rd);
    /*TODO: trying to figure out how to compute Carry bit*/
    if(Rd == 0)
        registers[get_register(CPSR)].Z=1;
    else if(Rd <0)
        registers[get_register(CPSR)].N=1;
}
// Rd:= Op1 AND NOT Op2
void Arm7tdmi::BICS(Arm7tdmi::_instruction ins)
{
    /*
    if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ~(op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ~((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ~(op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ~((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
   if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type,ins);
    Rd = Rn.word & ~op2;
    set_register((_registers)(ins.Rd),Rd);
    /*TODO: trying to figure out how to compute Carry bit*/
    if(Rd == 0)
        registers[get_register(CPSR)].Z=1;
    else if(Rd <0)
        registers[get_register(CPSR)].N=1;
}
void Arm7tdmi::EORS(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word ^ (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word ^ ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word ^ (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word ^ ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
   if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type,ins);
    Rd = Rn.word ^ op2;
    set_register((_registers)(ins.Rd),Rd);
    /*TODO: trying to figure out how to compute Carry bit*/
    if(Rd == 0)
        registers[get_register(CPSR)].Z=1;
    else if(Rd <0)
        registers[get_register(CPSR)].N=1;
}
void Arm7tdmi::MULS(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word * (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word * ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word * (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word * ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }*/
    if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rm = registers[get_register((_registers)(ins.Rm))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    Rd = Rm.word * Rs.word;
    set_register((_registers)(ins.Rd),Rd);
    /*TODO: trying to figure out how to compute Carry bit*/
    if(Rd == 0)
        registers[get_register(CPSR)].Z=1;
    else if(Rd <0)
        registers[get_register(CPSR)].N=1;
}
// Rd:= NOT Op2
void Arm7tdmi::MVNS(Arm7tdmi::_instruction ins)
{
   /* if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = ~(op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = ~((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = ~(op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = ~((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }*/
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = ~op2;
    set_register((_registers)(ins.Rd),Rd);
    //set_condition_code_flags(Rd, Rn.word, op2);
    if(Rd == 0){
        registers[get_register(CPSR)].Z=1;
    }else if(Rd <0)
        registers[get_register(CPSR)].N=1;
}

void Arm7tdmi::ORRS(Arm7tdmi::_instruction ins)
{
   /* if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word | (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word | ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word | (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word | ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word | op2;
    set_register((_registers)(ins.Rd),Rd);
    //set_condition_code_flags(Rd, Rn.word, op2);
    if(Rd == 0){
        registers[get_register(CPSR)].Z=1;
    }else if(Rd <0)
        registers[get_register(CPSR)].N=1;
}
void Arm7tdmi::SBCS(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word - (op2 << shift_amount) + registers[get_register(CPSR)].C - 1;
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word - ((uint32_t)op2 >> shift_amount) + registers[get_register(CPSR)].C - 1;
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word - (op2 >> shift_amount) + registers[get_register(CPSR)].C - 1;
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word - ((op2 >> shift_amount) | (op2 << (32 - shift_amount))) + registers[get_register(CPSR)].C - 1;
        break;
    }*/

    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word - op2 + registers[get_register(CPSR)].C - 1;
    set_register((_registers)(ins.Rd),Rd);
    set_condition_code_flags(Rd, Rn.word, op2);
}
// Reverse Sub  RSB - Rd:= Op2 - Op1
void Arm7tdmi::RSB(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = (op2 << shift_amount) - rn.word;
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = ((uint32_t)op2 >> shift_amount) - rn.word;
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = (op2 >> shift_amount) - rn.word;
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = ((op2 >> shift_amount) | (op2 << (32 - shift_amount))) - rn.word;
        break;
    }*/
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = op2 - Rn.word;
    set_register((_registers)(ins.Rd),Rd);
    set_condition_code_flags(Rd, Rn.word, op2);
}
void Arm7tdmi::RSC(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = (op2 << shift_amount) - rn.word + registers[get_register(CPSR)].C - 1;
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = ((uint32_t)op2 >> shift_amount) - rn.word + registers[get_register(CPSR)].C - 1;
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = (op2 >> shift_amount) - rn.word + registers[get_register(CPSR)].C - 1;
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = ((op2 >> shift_amount) | (op2 << (32 - shift_amount))) - rn.word + registers[get_register(CPSR)].C - 1;
        break;
    }*/
     if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = op2 - Rn.word + registers[get_register(CPSR)].C - 1;
    set_register((_registers)(ins.Rd),Rd);
    set_condition_code_flags(Rd, Rn.word, op2);
}
// Like ADD(with S flag) but it doesnt write in the register. It sets only the condition flags
void Arm7tdmi::CMN(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;
    int32_t tmp;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        tmp = rn.word + (op2 << shift_amount);
        break;
    case LR:
        tmp = rn.word + ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        tmp = rn.word + (op2 >> shift_amount);
        break;
    case RR:
        tmp = rn.word + ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }*/
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word + op2;

    set_condition_code_flags(Rd, Rn.word, op2);
}
// Like SUB(with S flag) but it doesnt write in the register. It sets only the condition flags
void Arm7tdmi::CMP(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction
    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;
    int32_t tmp;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        tmp = rn.word - (op2 << shift_amount);
        break;
    case LR:
        tmp = rn.word - ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        tmp = rn.word - (op2 >> shift_amount);
        break;
    case RR:
        tmp = rn.word - ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
   if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word - op2;
    
    set_condition_code_flags(Rd, Rn.word, op2);
}
// Like EOR(with S flag) but it doesnt write in the register. It sets only the condition flags
void Arm7tdmi::TEQ(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;
    int32_t tmp;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        tmp = rn.word ^ (op2 << shift_amount);
        break;
    case LR:
        tmp = rn.word ^ ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        tmp = rn.word ^ (op2 >> shift_amount);
        break;
    case RR:
        tmp = rn.word ^ ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
   if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word ^ op2;
    
    set_condition_code_flags(Rd, Rn.word, op2);
}
// Like AND(with S flag) but it doesnt write in the register. It sets only the condition flags
void Arm7tdmi::TST(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;
    int32_t tmp;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = rn.word & (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = rn.word & ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word & op2;
    
    set_condition_code_flags(Rd, Rn.word, op2);
}
void Arm7tdmi::MOVS(Arm7tdmi::_instruction ins)
{
    /*if (!evaluate_cond((_cond)ins.cond))
        return; // if the condition evaluates to true, then I execute the instruction

    _register_type rd, rn;
    int32_t op2;
    uint8_t shift_amount = 0;
    _shift shift_type;

    rd = registers[get_register((_registers)(ins.Rd))];
    rn = registers[get_register((_registers)(ins.Rn))];
    shift_type = (_shift)((ins.word >> 5) & 0x2); // isolating bit[5:6] to determine the type of shift

    if ((ins.opcode_id2 & 0x2) == 0x2)
    { // bit[25] aka I flag is set
        op2 = ins.word & 0xFF;
    }
    else
    { // No immediate operand(bit[25]/I flag unset)
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        if ((ins.word & 0x10) == 0x10)
        {                                         // bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount = (ins.word >> 8) & 0xF; // isolating bit[8:11]
        }
        else
        {                                          // bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount = (ins.word >> 7) & 0x1F; // isolating bit[7:11]
        }
    }
    switch (shift_type)
    {
    case LL:
        registers[get_register((_registers)ins.Rd)].word = (op2 << shift_amount);
        break;
    case LR:
        registers[get_register((_registers)ins.Rd)].word = ((uint32_t)op2 >> shift_amount);
        break;
    case AR:
        registers[get_register((_registers)ins.Rd)].word = (op2 >> shift_amount);
        break;
    case RR:
        registers[get_register((_registers)ins.Rd)].word = ((op2 >> shift_amount) | (op2 << (32 - shift_amount)));
        break;
    }
    */
   if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = op2;
    
    set_condition_code_flags(Rd, Rn.word, op2);
}   
#pragma endregion
#pragma region //Branch Operations
void Arm7tdmi::BX(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)(ins.cond)))
        return;

    // If the first bit of Rn(Rm in our case, and anyway it's the first bit of the instruction) is 1
    // then the mode gets switched to THUMB_MODE, otherwhise ARM_MODE
    if (ins.word & 0x1)
        set_mode(THUMB_MODE);
    else
        set_mode(ARM_MODE);
    // copying the content of the Rm register(first 4 bit of the instruction) into the PC(R15 is the PC)
    registers[get_register(R15)].word = registers[get_register((_registers)ins.Rm)].word;
}
void Arm7tdmi::B(Arm7tdmi::_instruction ins, _cond condition) {
    if (!evaluate_cond((_cond)(ins.cond))) return;
    int32_t offset = (ins.word&0x00FFFFFF)<<2;//isolating bit[0:23] and shifting left twice(offset<<2)
    if(ins.word&0x01000000)//if the Link bit is set(bit[24]), I save the old PC in the link register
        set_register(R15,registers[get_register(R15)].word);
    registers[get_register(R14)].word =+ offset;//adding the offset to PC
    return;
}
#pragma endregion
void Arm7tdmi::LDM(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDR(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRB(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRH(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRSB(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRSH(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::MLA(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::MRS(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::MSR(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SMLAL(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SMULL(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STM(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STR(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STRB(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STRH(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWI(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWP(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWPB(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::UMLAL(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::UMULL(Arm7tdmi::_instruction ins) {}
