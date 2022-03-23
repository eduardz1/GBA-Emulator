#include "headers/arm7tdmi.hh"
using namespace cpu;

#pragma region //ALU OPERATIONS

/**
 * @brief Add { Rd := Rn + Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::ADD_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word + op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Add with carry { Rd = Rn + Op2+ Carry }
 * 
 * @param ins instruction
 */
void Arm7tdmi::ADC_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word + op2+registers[get_register(CPSR)].C;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Subtract { Rd := Rn - Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::SUB_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word - op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief AND { Rd := Rn AND Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::AND_a(Arm7tdmi::_instruction ins)
{
    if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type,ins);
    Rd = Rn.word & op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief Bit Clear { Rd:= Op1 AND NOT Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::BIC_a(Arm7tdmi::_instruction ins)
{
   if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type,ins);
    Rd = Rn.word & ~op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief Exclusive OR { Rd := (Rn AND NOT Op2) OR (Op2 AND NOT Rn) }
 * 
 * @param ins instruction
 */
void Arm7tdmi::EOR_a(Arm7tdmi::_instruction ins)
{
   if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type,ins);
    Rd = Rn.word ^ op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief Move Negative Register { Rd := 0xFFFFFFFF EOR Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::MVN_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = 0xFFFFFFFF ^ op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief OR { Rd := Rn OR Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::ORR_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word | op2;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief Subtract with Carry { Rd := Rn - Op2 - 1 + Carry }
 * 
 * @param ins instruction
 */
void Arm7tdmi::SBC_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word - op2 + registers[get_register(CPSR)].C - 1;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Reverse Subtract { Rd:= Op2 - Rn }
 * 
 * @param ins instruction
 */
void Arm7tdmi::RSB_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = op2 - Rn.word;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Reverse Subtract with Carry { Rd:= Op2 - Rn - 1 + Carry }
 * 
 * @param ins instruction
 */
void Arm7tdmi::RSC_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = op2 - Rn.word + registers[get_register(CPSR)].C - 1;
    set_register((_registers)(ins.Rd),Rd);
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Compare Negative { CPSR flags := Rn + Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::CMN_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word + op2;

    set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Compare { CPSR flags := Rn - Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::CMP_a(Arm7tdmi::_instruction ins)
{
   if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word - op2;
    
    set_condition_code_flags(Rd, Rn.word, op2, true);
}

/**
 * @brief Test bitwise equality { CPSR flags := Rn EOR Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::TEQ_a(Arm7tdmi::_instruction ins)
{
   if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word ^ op2;
    
    set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief Test bits { CPSR flags := Rn AND Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::TST_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = Rn.word & op2;
    
    set_condition_code_flags(Rd, Rn.word, op2, false);
}

/**
 * @brief Move register or constant { Rd := Op2 }
 * 
 * @param ins instruction
 */
void Arm7tdmi::MOV_a(Arm7tdmi::_instruction ins)
{
   if (!evaluate_cond((_cond)ins.cond)) return;

    int32_t op2, Rd;
    _register_type Rn = registers[get_register((_registers)(ins.Rn))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    op2 = get_ALU_op2(shift_type, ins);
    Rd = op2;
    
    set_condition_code_flags(Rd, Rn.word, op2, false);
}

#pragma endregion
#pragma region //Mul operations

/**
 * @brief Multiply { Rd := Rm * Rs }
 * 
 * @param ins instruction
 */
void Arm7tdmi::MUL_a(Arm7tdmi::_instruction ins)
{
    if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t Rd;
    _register_type Rm = registers[get_register((_registers)(ins.Rm))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    Rd = Rm.word * Rs.word;
    set_register((_registers)(ins.Rn),Rd); // Rd is in position Rn
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rm.word, Rs.word, false);
}

/**
 * @brief Multiply Accumulate { Rd := (Rm *Rs) + Rn }
 * 
 * @param ins instruction
 */
void Arm7tdmi::MLA_a(Arm7tdmi::_instruction ins)
{
    if(!evaluate_cond((_cond)ins.cond)) return;

    int32_t Rd;
    _register_type Rm = registers[get_register((_registers)(ins.Rm))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rn = registers[get_register((_registers)(ins.Rd))];
    _shift shift_type = (_shift)((ins.word >> 5) & 0x2); // bits[5:6] determine the type of shift

    Rd = Rm.word * Rs.word + Rn.word;
    set_register((_registers)(ins.Rn),Rd); // Rd is in position Rn
    if(ins.word & 0x00100000) // flag S set
        set_condition_code_flags(Rd, Rm.word, Rs.word, false);
}

/**
 * @brief Signed Multiply Long with Accumulate  { Rd := (Rm * Rs) + Rn }
 * @code
 * SMLAL{cond}{S} RdLo,RdHi,Rm,Rs
 * @endcode
 * 
 * 
 * @param ins instruction
 */
void Arm7tdmi::SMLAL(Arm7tdmi::_instruction ins) {
    if(!evaluate_cond((_cond)ins.cond)) return;

    int64_t rd;
    
    _register_type Rd_hi = registers[get_register((_registers)(ins.Rn))];
    _register_type Rd_lo = registers[get_register((_registers)(ins.Rd))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];
    
    rd=Rm.word*Rs.word + ((Rd_hi.word<<32) | (Rd_lo.word));
    set_register((_registers)(ins.Rd),(rd)&0xFFFFFFFF);//saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn),(rd>>32)&0xFFFFFFFF);//saving upper 32 bits in rd_high(aka bit[16..19])

    if(ins.word & 0x00100000) // flag S set
    {
        // set_condition_code_flags(rd, Rm.word, Rs.word, false);
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0)  ? 1 : 0;
    }
}
/**
 * @brief Signed Multiply Long { Rd := Rm * Rs }
 * 
 * @param ins 
 */
void Arm7tdmi::SMULL(Arm7tdmi::_instruction ins) {
    if(!evaluate_cond((_cond)ins.cond)) return;

    int64_t rd;
    
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];
    
    rd=Rm.word*Rs.word;
    set_register((_registers)(ins.Rd),(rd)&0xFFFFFFFF);//saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn),(rd>>32)&0xFFFFFFFF);//saving upper 32 bits in rd_high(aka bit[16..19])

    if(ins.word & 0x00100000) // flag S set
    {
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0)  ? 1 : 0;
    }
}
/**
 * @brief Unsigned Multiply Long with Accumulate { Rd := (Rm * Rs) + Rn}
 * 
 * @param ins instruction
 */
void Arm7tdmi::UMLAL(Arm7tdmi::_instruction ins) {
    if(!evaluate_cond((_cond)ins.cond)) return;

    uint64_t rd;
    
    _register_type Rd_hi = registers[get_register((_registers)(ins.Rn))];
    _register_type Rd_lo = registers[get_register((_registers)(ins.Rd))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];
    
    rd=(uint32_t)Rm.word*(uint32_t)Rs.word + (uint32_t)(((uint32_t)Rd_hi.word<<32) | (uint32_t(Rd_lo.word)));
    set_register((_registers)(ins.Rd),(rd)&0xFFFFFFFF);//saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn),(rd>>32)&0xFFFFFFFF);//saving upper 32 bits in rd_high(aka bit[16..19])

    if(ins.word & 0x00100000) // flag S set
    {
        // set_condition_code_flags(rd, Rm.word, Rs.word, false);
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0)  ? 1 : 0;
    }
}
/**
 * @brief Unsigned Multiply Long {Rd := Rm * Rs}
 * 
 * @param ins instruction
 */
void Arm7tdmi::UMULL(Arm7tdmi::_instruction ins) {
    if(!evaluate_cond((_cond)ins.cond)) return;

    uint64_t rd;
    
    //_register_type Rd_hi = registers[get_register((_registers)(ins.Rn))];
    //_register_type Rd_lo = registers[get_register((_registers)(ins.Rd))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];
    
    rd=(uint32_t)Rm.word*(uint32_t)Rs.word;
    set_register((_registers)(ins.Rd),(rd)&0xFFFFFFFF);//saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn),(rd>>32)&0xFFFFFFFF);//saving upper 32 bits in rd_high(aka bit[16..19])

    if(ins.word & 0x00100000) // flag S set
    {
        // set_condition_code_flags(rd, Rm.word, Rs.word, false);
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0)  ? 1 : 0;
    }
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
#pragma region //Single Data Transfer
void Arm7tdmi::LDR(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRB(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STR(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STRB(Arm7tdmi::_instruction ins) {}
#pragma endregion
#pragma region //Halfword and Signed Data Transfer
void Arm7tdmi::LDRH(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STRH(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRSB(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRSH(Arm7tdmi::_instruction ins) {}
#pragma endregion
#pragma region //Block Data Transfer
void Arm7tdmi::STM(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDM(Arm7tdmi::_instruction ins) {}
#pragma endregion
#pragma region //PSR Transfer
void Arm7tdmi::MRS(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::MSR(Arm7tdmi::_instruction ins) {}
#pragma endregion
void Arm7tdmi::SWI(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWP(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWPB(Arm7tdmi::_instruction ins) {}

