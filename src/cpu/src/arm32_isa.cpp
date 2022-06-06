#include "../include/GBA-emulator/cpu/arm7tdmi.hh"
/*TODO: figuring out a way to pass the bus controller/ram to a method so that it can load/store data*/
#pragma region // ALU OPERATIONS

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
#pragma region // Mul operations

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
 *  Signed Multiply Long with Accumulate  { Rd := (Rm * Rs) + Rn }
 *  
 * @param ins instruction
 */
void Arm7tdmi::SMLAL_a(Arm7tdmi::_instruction ins) 
{
    if(!evaluate_cond((_cond)ins.cond)) return;

    int64_t rd; 
    _register_type Rd_hi = registers[get_register((_registers)(ins.Rn))];
    _register_type Rd_lo = registers[get_register((_registers)(ins.Rd))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];

    rd = Rm.word * Rs.word + (int64_t)(((int64_t)Rd_hi.word << 32) | (Rd_lo.word));
    set_register((_registers)(ins.Rd), (rd) & 0xFFFFFFFF); // saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn), (rd >> 32) & 0xFFFFFFFF); // saving upper 32 bits in rd_high(aka bit[16..19])

    if(ins.word & 0x00100000) // flag S set
    {
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0)  ? 1 : 0;
    }
}

/**
 * @brief Signed Multiply Long { Rd := Rm * Rs }
 * 
 * @param ins instruction
 */
void Arm7tdmi::SMULL_a(Arm7tdmi::_instruction ins) 
{
    if(!evaluate_cond((_cond)ins.cond)) return;

    int64_t rd;
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];
    
    rd=Rm.word*Rs.word;
    set_register((_registers)(ins.Rd),(rd)&0xFFFFFFFF); // saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn),(rd>>32)&0xFFFFFFFF); // saving upper 32 bits in rd_high(aka bit[16..19])

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
void Arm7tdmi::UMLAL_a(Arm7tdmi::_instruction ins) 
{
    if(!evaluate_cond((_cond)ins.cond)) return;

    uint64_t rd;
    _register_type Rd_hi = registers[get_register((_registers)(ins.Rn))];
    _register_type Rd_lo = registers[get_register((_registers)(ins.Rd))];
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];

    rd = (uint32_t)Rm.word * (uint32_t)Rs.word + (uint64_t)(((uint64_t)Rd_hi.word << 32) | (uint32_t(Rd_lo.word)));
    set_register((_registers)(ins.Rd), (rd)&0xFFFFFFFF);         // saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn), (rd >> 32) & 0xFFFFFFFF); // saving upper 32 bits in rd_high(aka bit[16..19])

    if (ins.word & 0x00100000) // flag S set
    {
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0) ? 1 : 0;
    }
}

/**
 * @brief Unsigned Multiply Long {Rd := Rm * Rs}
 * 
 * @param ins instruction
 */
void Arm7tdmi::UMULL_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)ins.cond)) return;

    uint64_t rd;
    _register_type Rs = registers[get_register((_registers)(ins.Rs))];
    _register_type Rm = registers[get_register((_registers)(ins.Rd))];

    rd = (uint32_t)Rm.word * (uint32_t)Rs.word;
    set_register((_registers)(ins.Rd), (rd)&0xFFFFFFFF);         // saving lower 32 bits in rd_low(aka bit[12..15])
    set_register((_registers)(ins.Rn), (rd >> 32) & 0xFFFFFFFF); // saving upper 32 bits in rd_high(aka bit[16..19])

    if (ins.word & 0x00100000) // flag S set
    {
        registers[get_register(CPSR)].Z = (rd == 0) ? 1 : 0;
        registers[get_register(CPSR)].N = (rd < 0) ? 1 : 0;
    }
}

#pragma endregion
#pragma region // Branch Operations

/**
 * @brief Branch and Exchange { R15 := Rn, T bit := Rn[0] }
 * 
 * @details
 *  The instruction is only executed if the condition is true.
 *  This instruction performs a branch by copying the contents of a general register, Rn,
 *  into the program counter, PC. The branch causes a pipeline flush and refill from the
 *  address specified by Rn. This instruction also permits the instruction set to be
 *  exchanged. When the instruction is executed, the value of Rn[0] determines whether
 *  the instruction stream will be decoded as ARM or THUMB instructions.
 * 
 * @param ins instruction
 */
void Arm7tdmi::BX_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)(ins.cond))) return;

    // Check first bit of Rn
    if (ins.word & 0x1)
        set_mode(THUMB_MODE);
    else
        set_mode(ARM_MODE);
    
    registers[get_register(R15)].word = registers[get_register((_registers)ins.Rm)].word;
}

/**
 * @brief Branch { R15 := address }
 * 
 * @details
 *  The instruction is only executed if the condition is true.
 *  Branch instructions contain a signed 2's complement 24 bit offset. This is shifted left
 *  two bits, sign extended to 32 bits, and added to the PC. The instruction can therefore
 *  specify a branch of +/- 32Mbytes. The branch offset must take account of the prefetch
 *  operation, which causes the PC to be 2 words (8 bytes) ahead of the current
 *  instruction.
 *  Branches beyond +/- 32Mbytes must use an offset or absolute destination which has
 *  been previously loaded into a register. In this case the PC should be manually saved
 *  in R14 if a Branch with Link type operation is required.
 * 
 * @param ins instruction
 */
void Arm7tdmi::B_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)(ins.cond))) return;

    int32_t offset = (ins.word << 8) >> 6; // shift left then right to extend sign of offset and add 2bit left shift
    registers[get_register(R15)].word += offset;
}

/**
 * @brief Branch with Link { R14 := R15, R15 := address }
 * 
 * @details
 *  The instruction is only executed if the condition is true.
 *  Branch with Link (BL) writes the old PC into the link register (R14) of the current bank.
 *  The PC value written into R14 is adjusted to allow for the prefetch, and contains the
 *  address of the instruction following the branch and link instruction. Note that the CPSR
 *  is not saved with the PC and R14[1:0] are always cleared.
 *  To return from a routine called by Branch with Link use MOV PC,R14 if the link register
 *  is still valid or   LDM Rn!,{..PC} if the link register has been saved onto a stack pointed
 *  to by Rn.
 * 
 * @param ins instruction
 */
void Arm7tdmi::BL_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)(ins.cond))) return;

    int32_t offset = (ins.word << 8) >> 6; // shift left then right to extend sign of offset and add 2bit left shift
    if (ins.word & 0x01000000)
        set_register(R14, registers[get_register(R15)].word);

    registers[get_register(R15)].word += offset;
}

#pragma endregion
#pragma region // Single Data Transfer

/**
 * @brief Load register from memory { Rd := (address) }
 * 
 * @details
 *  The instruction is only executed if the condition is true.
 *  The single data transfer instructions are used to load or store single bytes or words of
 *  data. The memory address used in the transfer is calculated by adding an offset to or
 *  subtracting an offset from a base register.
 *  The result of this calculation may be written back into the base register if auto-indexing
 *  is required.
 *
 * @param ins instruction
 */
void Arm7tdmi::LDR_a(Arm7tdmi::_instruction ins)
{
    if (!evaluate_cond((_cond)(ins.cond))) return;

    _register_type base_register = registers[get_register((_registers)ins.Rn)];
    _register_type rd = registers[get_register((_registers)(ins.Rd))];
   
    int32_t offset = (ins.opcode_id2 & 0x20) ? // Check I flag 
                     ins.word & 0xFFF : 
                     get_ALU_op2((_shift)((ins.word >> 0x5) & 0x2), ins);

    int32_t base_offset;
    int32_t base_tmp = base_register.word; // saving the original base register for post_index

    base_offset = (ins.word & 0x800000) ? base_register.word + offset : base_register.word - offset;
    /* bit[24] set   -> pre-index:   add offset to base register before loading
               clear -> post-index:  add offset to base register after  loading */
    if (ins.word & 0x1000000)
    { // bit[24] set
        // read_from_memory()
    }
}

void Arm7tdmi::LDRB_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STR_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STRB_a(Arm7tdmi::_instruction ins) {}
#pragma endregion
#pragma region //Halfword and Signed Data Transfer
void Arm7tdmi::LDRH_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::STRH_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRSB_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDRSH_a(Arm7tdmi::_instruction ins) {}
#pragma endregion
#pragma region //Block Data Transfer
void Arm7tdmi::STM_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::LDM_a(Arm7tdmi::_instruction ins) {}
#pragma endregion
#pragma region //PSR Transfer
void Arm7tdmi::MRS_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::MSR_a(Arm7tdmi::_instruction ins) {}
#pragma endregion
void Arm7tdmi::SWI_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWP_a(Arm7tdmi::_instruction ins) {}
void Arm7tdmi::SWPB_a(Arm7tdmi::_instruction ins) {}

