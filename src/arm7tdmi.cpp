#include "headers/arm7tdmi.hh"

Arm7tdmi::Arm7tdmi(){
    /*registers[CPSR].word=2882112257;
    registers[CPSR].M0=0;
    std::cout<<"After cambio"<<(uint)registers[CPSR].word<<std::endl;*/
	build_THUMB_isa();
}

Arm7tdmi::~Arm7tdmi(){}

Arm7tdmi::_mode Arm7tdmi::get_mode()
{
    // 0x00000003 & R[15] == 0 -> ARM_MODE
    // 0x00000003 & R[15] == 2 -> THUMB_MODE
    return registers[R15].word & 0x00000003 ? THUMB_MODE : ARM_MODE;
}

/**
 * @brief returns the register corresponding to the
 * mode the system is in (FIQ, IRQ, ecc...)
 * 
 * @param id 4/3 bit identifier of the register we are trying to access
 */
Arm7tdmi::_registers Arm7tdmi::get_register(Arm7tdmi::_registers id)
{
    _access_mode mode = get_access_mode();
    switch(mode)
    {
    case FIQ: {
        switch(id)
        {
        case R8: return R8_fiq;
        case R9: return R9_fiq;
        case R10: return R10_fiq;
        case R11: return R11_fiq;
        case R12: return R12_fiq;
        case R13: return R13_fiq;
        case R14: return R14_fiq;
        //case CPSR: return SPSR_fiq;
        default: return id;
        }
    }
    case IRQ: {
        switch(id)
        {
        case R13: return R13_irq;
        case R14: return R14_irq;
        //case CPSR: return SPSR_irq;
        default: return id;
        }
    }
    case SVC: {
        switch(id)
        {
        case R13: return R13_svc;
        case R14: return R14_svc;
        //case CPSR: return SPSR_svc;
        default: return id;
        }
    }
    case ABT: {
        switch(id)
        {
        case R13: return R13_abt;
        case R14: return R14_abt;
        //case CPSR: return SPSR_abt;
        default: return id;
        }
    }
    case UND: {
        switch(id)
        {
        case R13: return R13_und;
        case R14: return R14_und;
        //case CPSR: return SPSR_und;
        default: return id;
        }
    }
    default: return id; // SYS/USR
    }
}

/**
 * @brief On entry: \n 
 * 1a - Preserves the address of the next instruction in the appropriate
 *     Link Register, next instruction is specified by the offset parameter \n 
 * 2a - Copies CPSR into the appropriate SPSR \n 
 * 3a - Forces the CPSR mode bits to a value which depends on the exception \n 
 * 4a - Forces the PC to fetch the next instruction from the relevant exception vector \n 
 * Entering from THUMB state forces a switch into ARM state when PC is loeaded 
 * with the exception vector address
 * 
 * On completion: \n 
 * 1b - Moves the Link Register, minus an offset where appropriate, to the PC \n 
 * 2b - Copies SPSR back to CPSR \n 
 * 3b - Clears the interrupt disable flags, if they were set on entry
 */
void Arm7tdmi::exception_handler(/* _exceptions EXC */)
{
    /*
     * offset value is 4 or 8 (bytes) depending on the exception when entering from ARM
     * state, in THUMB the exception handler is responsible for choosing an offset
     * such that the program resumes from the correct place on return from an exception
     */
    uint8_t offset;
    _registers link = get_register(R14);
    _registers PC = get_register(R15); // program counter
    _registers SPSR;

    if(get_mode() == ARM_MODE)
    {
        // 1a
        registers[link].word=registers[PC+4].word;//or +8, still gotta handle that
    }
    else // THUMB_MODE
    {
        // 1a
    }
    // 2a
    switch(get_access_mode())
    {
    case FIQ: SPSR = SPSR_fiq; break;
    case IRQ: SPSR = SPSR_irq; break;
    case SVC: SPSR = SPSR_svc; break;
    case ABT: SPSR = SPSR_abt; break;
    case UND: SPSR = SPSR_und; break;
    default : SPSR = CPSR; break; // SYS/USR
    }
    registers[SPSR] = registers[CPSR];

    // 3a - switch(exception){}
    // 4a - registers[PC] = exceptions[EXC]; with "exceptions" being an array of function pointers
}

/**
 * @brief sets/clears Zero (Z), Negative (N), Overflow (V) and Carry (C) flag 
 * 
 * @param Rd destination register (result of sum/sub)
 * @param Rn first operand
 * @param op2 second operand
 * @param overflowable set to "true" if the operation can cause an overflow
 */
void Arm7tdmi::set_condition_code_flags(int32_t Rd, int32_t Rn, int32_t op2, bool overflowable)
{
    registers[get_register(CPSR)].Z = (Rd == 0) ? 1 : 0;
    registers[get_register(CPSR)].N = (Rd < 0)  ? 1 : 0;

    if(!overflowable) return;

    if((Rn > 0 && op2 > 0 && Rd <= 0) || (Rn < 0 && op2 < 0 && Rd >= 0)) // if true sets overflow flag
        registers[get_register(CPSR)].V = 1;
    else
        registers[get_register(CPSR)].V = 0;

    if((Rn > 0 || op2 > 0 && Rd <= 0) || (Rn < 0 && op2 < 0 && Rd >= 0)) // if true sets carry flag
        registers[get_register(CPSR)].C = 1;
    else
        registers[get_register(CPSR)].C = 0;
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

/*Not sure if the current mode I am in has to be taken into account when fetching the new mode
  Aka, should the mode be got in the SPSR_svc register if i am in the supervisor register? be got */
Arm7tdmi::_access_mode Arm7tdmi::get_access_mode(){ return (_access_mode)(registers[CPSR].word & 0xF); }

/**
 * @brief Calculates value of second operand
 * 
 * @details 
 *  Based on [I] flag, assigns second operand either
 *  to an immediate value (I set) or to the content of the register
 *  pointed by op2. Shifts the result as specified by the instruction and
 *  computes the carry carry based on this scheme:
 * 
 *  Carry out \n 
 *      |----lsl = bit[(31 - (#bit_shift) + 1)] \n
 *      |----lsr = bit[(#bit_shift) - 1] \n
 *      |----asr = bit[(#bit_shift) - 1] \n
 *      |----ror = bit[(#bit_shift) - 1] \n 
 * 
 * @param type type of shift performed
 * @param ins instruction 
 * @return second operand 
 */
int32_t Arm7tdmi::get_ALU_op2(_shift type, _instruction ins)
{
    uint8_t shift;
    int32_t op2;
    
    if ((ins.opcode_id2 & 0x20) == 0x20) // bit[25] aka I flag is set
    { 
        op2 = ins.word & 0xFF;
        shift = (ins.word & 0xF00) >> 7; // double the "rotate" field value
        op2 = ((op2 >> shift) | (op2 << (32 - shift)));
        if (ins.word & 0x00100000)
            registers[get_register(CPSR)].C = (op2 >> (shift - 1)) & 1;
    }
    else // No immediate operand(bit[25]/I flag unset)
    {
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        shift = (ins.word & 0x10) ? 
                (ins.word >> 8) & 0xF : // [4] set -> shift amount specified by the bottom byte of Rs
                (ins.word >> 7) & 0x1F; // [4] clear -> shift amount is a 5 bit unsigned integer
        
        switch (type)
        {
        case LL:
            op2 = op2 << shift;
            if (ins.word & 0x00100000)
                registers[get_register(CPSR)].C = ((op2 >> (32 - shift)) & 1);
            break;
        case LR:
            op2 = (uint32_t)op2 >> shift;
            if (ins.word & 0x00100000)
                registers[get_register(CPSR)].C = (op2 >> (shift - 1)) & 1;
            break;
        case AR:
            op2 = op2 >> shift;
            if (ins.word & 0x00100000)
                registers[get_register(CPSR)].C = (op2 >> (shift - 1)) & 1;
            break;
        case RR: default:
            op2 = ((op2 >> shift) | (op2 << (32 - shift)));
            if (ins.word & 0x00100000)
                registers[get_register(CPSR)].C = (op2 >> (shift - 1)) & 1;
            break;
        }
    }
    return op2;
}

/**
 * @brief Checks,based on the condition, which condition must be evaluated
 * 
 * @param condition condition of the instruction(uppermost 4 bits)
 * @return  true if the condition evaluates to true
 * @return  false if the condition evaluates to false
 */
bool Arm7tdmi::evaluate_cond(Arm7tdmi::_cond condition)
{
    _register_type cpsr = registers[CPSR];
    switch(condition)
    {
    case EQ: return cpsr.Z; // Z set
    case NE: return !cpsr.Z;// Z clear
    case CS: return cpsr.C; // C set
    case CC: return !cpsr.C;// C clear
    case MI: return cpsr.N; // N set
    case PL: return !cpsr.N;// N clear
    case VS: return cpsr.V; // V set
    case VC: return !cpsr.V;// V clear
    case HI: return cpsr.C && !cpsr.Z; // C set && Z clear
    case LS: return !cpsr.C || cpsr.Z; // C clear || Z set
    case GE: return cpsr.N == cpsr.V;
    case LT: return cpsr.N != cpsr.V;
    case GT: return !cpsr.Z && (cpsr.N == cpsr.V);
    case LE: return cpsr.Z || (cpsr.N != cpsr.V);
    case AL: default: return true;
    }
}

void Arm7tdmi::set_register(Arm7tdmi::_registers reg, uint32_t val)
{
    registers[reg].word = val;
}


// Passing an entire bus is too memory heavy, we need to implement it in another way
/* If fetching in THUMB_MODE, I have to read only the lower half */
uint32_t Arm7tdmi::fetch(Bus bus_controller){
        if(get_mode() == THUMB_MODE)
            return bus_controller.RAM[R15] & 0x0000FFFF;
        else 
            return bus_controller.RAM[R15];
}

/**
 * @brief Wrapper for @c decode_executeTHUMB() and @c decode_executeARM32()
 * @see decode_executeTHUMB()
 * @see decode_executeARM32()
 * @param ins Instruction to be decoded and executed. It can be decoded either in ARM mode or in THUMB mode
 */
void Arm7tdmi::decode_execute(Arm7tdmi::_instruction ins)
{
    if(get_mode() == THUMB_MODE)
        (this->*THUMB_isa[ins.word])(ins); // check std::invoke syntax for a prettier solution
    else
        decode_executeARM32(ins);
}

/**
 * @brief saves every possible value of a 16 bit instruction in an array of function pointers to the instruction
 * 
 */
void Arm7tdmi::build_THUMB_isa()
{
    for(_instruction i = {0}; i.word < 65536; i.word++)
        THUMB_isa[i.word] = decode_THUMB(i);
}

/**
 * @brief decodes a THUMB instruction and returns the function pointer to the corresponding function
 * 
 * @param ins instruction to be decoded
 * @return Arm7tdmi::instruction_ptr pointer to the function that implements the instruction
 */
Arm7tdmi::instruction_ptr Arm7tdmi::decode_THUMB(Arm7tdmi::_instruction ins)
{
    // Undef when [25:27] -> 011 && [4] set
    if((ins.word & 0x0E000010) == 0x06000010) return &Arm7tdmi::undef;

    uint8_t hi_byte = ins.halfword_lo >> 8; // isolate hi byte

    switch(hi_byte >> 4) // isolate higher 4 bits to make it more readable (bits[12:15])
    {
    case 0x0: {
        switch(hi_byte)//bits[8:15]
        {
        case 0x00: case 0x01: case 0x02: case 0x03: 
        case 0x04: case 0x05: case 0x06: case 0x07: return &Arm7tdmi::LSL_t; // 0000 0xxx
        case 0x08: case 0x09: case 0x0C: case 0x0D:
        case 0x0A: case 0x0B: case 0x0E: case 0x0F: return &Arm7tdmi::LSR_t; // 0000 1xxx
        }
    }
    case 0x1: {
        switch(hi_byte)//bits[8:15]
        {
        case 0x10: case 0x11: case 0x12: case 0x13: 
        case 0x14: case 0x15: case 0x16: case 0x17: return &Arm7tdmi::ASR_t;  // 0001 0xxx
        case 0x18: case 0x19: case 0x1C: case 0x1D: return &Arm7tdmi::ADD_t; // 0001 1x0x
        case 0x1A: case 0x1B: case 0x1E: case 0x1F: return &Arm7tdmi::SUB_t; // 0001 1x1x
        }
    }
    case 0x2: {
        switch(hi_byte)
        {
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27: return &Arm7tdmi::MOV_t; // 0010 0xxx
        case 0x28: case 0x29: case 0x2C: case 0x2D:
        case 0x2A: case 0x2B: case 0x2E: case 0x2F: return &Arm7tdmi::CMP_t;  // 0010 1xxx
        }
    }
    case 0x3: {
        switch(hi_byte)
        {
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27: return &Arm7tdmi::ADD_t; // 0011 0xxx
        case 0x28: case 0x29: case 0x2C: case 0x2D:
        case 0x2A: case 0x2B: case 0x2E: case 0x2F: return &Arm7tdmi::SUB_t; // 0011 1xxx
        }
    }
    case 0x4: {
        switch((hi_byte & 0x0C))
        {
        case 0x0: { // ALU operations
            switch((ins.halfword_lo & 0x03C0) >> 6) // isolate opcode
            {
            case 0x0: return &Arm7tdmi::AND_t;
            case 0x1: return &Arm7tdmi::EOR_t;
            case 0x2: return &Arm7tdmi::LSL_t;
            case 0x3: return &Arm7tdmi::LSR_t;
            case 0x4: return &Arm7tdmi::ASR_t;
            case 0x5: return &Arm7tdmi::ADC_t;
            case 0x6: return &Arm7tdmi::SBC_t;
            case 0x7: return &Arm7tdmi::ROR_t;
            case 0x8: return &Arm7tdmi::TST_t;
            case 0x9: return &Arm7tdmi::NEG_t;
            case 0xA: return &Arm7tdmi::CMP_t;
            case 0xB: return &Arm7tdmi::CMN_t;
            case 0xC: return &Arm7tdmi::ORR_t;
            case 0xD: return &Arm7tdmi::MUL_t;
            case 0xE: return &Arm7tdmi::BIC_t;
            case 0xF: return &Arm7tdmi::MVN_t;
            }
        }  
        case 0x1: { // High Register operations/Branch Exchange
            switch(hi_byte & 0x03) // isolate opcode
            {
            case 0x0: return &Arm7tdmi::ADD_t;
            case 0x1: return &Arm7tdmi::CMP_t;
            case 0x2: return &Arm7tdmi::MOV_t;
            case 0x3:
                if (ins.halfword_lo & 0x0080) // H0[0]
                    return &Arm7tdmi::BX_t;
                else
                    return &Arm7tdmi::undef;
            }
        }
        default: return &Arm7tdmi::LDR_t; // PC-relative Load
        } 
    }
    case 0x5: {
        switch(hi_byte & 0x0E) // isolate [9:11]
        {   // Load/Store with Register Offset
        case 0x0: return &Arm7tdmi::STR_t;
        case 0x2: return &Arm7tdmi::STRB_t;
        case 0x4: return &Arm7tdmi::LDR_t;
        case 0x6: return &Arm7tdmi::LDRB_t;
            // Load/Store Sign-Extended Byte/Halfword
        case 0x1: return &Arm7tdmi::STRH_t;
        case 0x3: return &Arm7tdmi::LDRH_t;
        case 0x5: return &Arm7tdmi::LDRSB_t; // technically should be LDSB but it seems completely equivalent to ARM ::= LDSRB
        case 0x7: return &Arm7tdmi::LDRSH_t; // same as above
        }
    }
    case 0x6: case 0x7: { // Load/Store with Immediate Offset
        switch(hi_byte & 0x18) // isolate B and L flags
        {
        case 0x00: return &Arm7tdmi::STR_t;
        case 0x08: return &Arm7tdmi::LDR_t;
        case 0x10: return &Arm7tdmi::STRB_t;
        case 0x18: return &Arm7tdmi::LDRB_t;
        }
    }
    case 0x8: // Load/Store Halfword
        if ((hi_byte & 0x08) == 0) // L[0]
            return &Arm7tdmi::STRH_t; 
        else 
            return &Arm7tdmi::LDRH_t;  
    case 0x9: // SP-relative Load/Store
        if ((hi_byte & 0x08) == 0) // L[0]
            return &Arm7tdmi::STR_t; 
        else 
            return &Arm7tdmi::LDR_t; 
    case 0xA: return &Arm7tdmi::ADD_a; // Load Address
    case 0xB: {
        switch(hi_byte)
        {   // Add Offset to Stack Pointer
        case 0xB0: return &Arm7tdmi::ADD_t;
        case 0xB1: return &Arm7tdmi::SUB_t;
            // Push/Pop Registers
        case 0xB4: case 0xB5: return &Arm7tdmi::PUSH_t;
        case 0xBA: case 0xBB: return &Arm7tdmi::POP_t;
        default: return &Arm7tdmi::undef;
        }
    }
    case 0xC: // Multiple Load/Store
        if ((hi_byte & 0x08) == 0) // L[0]
            return &Arm7tdmi::STMIA_t;
        else
            return &Arm7tdmi::LDMIA_t;
    
    case 0xD:
        switch(hi_byte & 0x0F) // cond
        {
        case 0x0F: return &Arm7tdmi::SWI_t;
        case 0x0E: return &Arm7tdmi::undef;
        default: return &Arm7tdmi::B_t;
        }
    case 0xE:
        if((hi_byte & 0x08) == 0x00)
            return &Arm7tdmi::B_t;
        else
            return &Arm7tdmi::undef;
    case 0xF:
        if((hi_byte & 0x08) == 0x00) // H[0]
            return &Arm7tdmi::BL_t;
        else // ??? not specified ffs, I assume BL
            return &Arm7tdmi::BL_t;
    }
    return &Arm7tdmi::undef;
}

/*Decoding a single ARM32 instruction*/
void Arm7tdmi::decode_executeARM32(Arm7tdmi::_instruction ins)
{
    // Undef when [25:27] -> 011 && [4] set
    if((ins.word & 0x0E000010) == 0x06000010)
    {
        undef(ins);
        return;
    }

    uint8_t opcode;
    opcode = (ins.opcode_id2 & 0x1E) >> 1; //isolating opcode bits

    switch(ins.opcode_id2 >> 6) // check bits [26:27]
    {  
    case 0x0: { 
        // Data Processing / PSR Transfer 
        if (((ins.opcode_id2 & 0x20) == 0x20) ||
            ((ins.opcode_id1 & 0x01) == 0x00) || 
            ((ins.opcode_id1 & 0x08) == 0x00) )
        {
            switch(opcode)
            {
            case 0x0: AND_a(ins); break;
            case 0x1: EOR_a(ins); break;
            case 0x2: SUB_a(ins); break;
            case 0x3: RSB_a(ins); break;
            case 0x4: ADD_a(ins); break;
            case 0x5: ADC_a(ins); break;
            case 0x6: SBC_a(ins); break;
            case 0x7: RSC_a(ins); break;
            case 0x8: {
                    if (ins.opcode_id2 & 0x1) // S[1]
                        TST_a(ins);
                    else if ((ins.word & 0x020F0FFF) == 0x000F0000) // I[0] && SBO [16:19] && SBZ [0:11]
                        MRS_a(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0x9: {
                    if (ins.opcode_id2 & 0x1) // S[1]
                        TEQ_a(ins);
                    else if ((ins.word & 0x0200F000) == 0x0200F000) // I[1] && SBO [12:15]
                        MSR_a(ins);
                    else if ((ins.word & 0x0200FFF0) == 0x0000F000) // I[0] && SBO [11:15] && SBZ [4:11]
                        MSR_a(ins);
                    else if ((ins.word & 0x020FFF10) == 0x000FFF10) // I[0] && SBO [8:19] && [4] set
                        BX_a(ins);
                    else
                        undef(ins);
                    break; 
                }
            case 0xA: {
                    if ((ins.word & 0x0010F000) == 0x00100000) // S[1] && SBZ [12:15]
                        CMP_a(ins);
                    else if ((ins.word & 0x020F0FFF) == 0x000F0000) // I[0] && SBO [16:19] && SBZ [0:11]
                        MRS_a(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0xB: {
                    if ((ins.word & 0x0010F000) == 0x00100000) // S[1] && SBZ [12:15]
                        CMN_a(ins);
                    else if ((ins.word & 0x0200F000) == 0x0200F000) // I[1] && SBO [12:15]
                        MSR_a(ins);
                    else if ((ins.word & 0x0200FFF0) == 0x0000F000) // I[0] && SBO [11:15] && SBZ [4:11]
                        MSR_a(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0xC: ORR_a(ins); break;
            case 0xD: {
                    if ((ins.word & 0xF0000) == 0x00000) // SBZ [16:19]
                        MOV_a(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0xE: BIC_a(ins); break;
            case 0xF: {
                    if ((ins.word & 0xF0000) == 0x00000) // SBZ [16:19]
                        MVN_a(ins);
                    else
                        undef(ins);
                    break;
                }
            }
            return;
        }

        switch(ins.opcode_id1)
        {
        case 0x9:
            switch(ins.opcode_id2)
            {   // Multiply       
            case 0x0: case 0x1: MUL_a(ins); break;
            case 0x2: case 0x3: MLA_a(ins); break;
                // Multiply Long
            case 0x8: case 0x9: UMULL_a(ins); break;
            case 0xA: case 0xB: UMLAL_a(ins); break;
            case 0xC: case 0xD: SMULL_a(ins); break;
            case 0xE: case 0xF: SMLAL_a(ins); break;
                // Single Data Swap
            case 0x10: SWP_a(ins); break;
            case 0x14: SWPB_a(ins); break;
            }
            break;

        case 0xB:
            if((ins.opcode_id2 & 1) == 1)
                LDRH_a(ins);
            else
                STRH_a(ins);
            break;

        case 0xD:
            if((ins.opcode_id2 & 1) == 1)
                LDRSB_a(ins);
            else
                undef(ins);
            break;

        case 0xF:
            if((ins.opcode_id2 & 1) == 1)
                LDRSH_a(ins);
            else
                undef(ins);
            break;
        }
        break;
    }

    case 0x1: {
        switch(ins.opcode_id2 & 0x5) // isolate bits 20 and 22
        {
        case 0x0: STR_a(ins); break;  // xxxx x0x0
        case 0x1: LDR_a(ins); break;  // xxxx x0x1
        case 0x4: STRB_a(ins); break; // xxxx x1x0
        case 0x5: LDRB_a(ins); break; // xxxx x1x1
        }
        break;
    }

    case 0x2: {
        switch(ins.opcode_id2 & 0x21) // isolate bits 20 and 25
        {
        case 0x20: case 0x21: B_a(ins); break; // xx1x xxx0 U xx1x xxx1
        case 0x01: LDM_a(ins); break; // xx0x xxx1
        case 0x00: STM_a(ins); break; // xx0x xxx0
        }
        break;
    }

    case 0x3: {   
        if(ins.opcode_id2 & 0x30)
            SWI_a(ins);
        else 
            undef(ins);
        break;
    }
    }
}
