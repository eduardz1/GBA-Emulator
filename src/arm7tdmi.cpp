#include "headers/arm7tdmi.hh"

using namespace cpu;
Arm7tdmi::Arm7tdmi(){
    /*registers[CPSR].word=2882112257;
    registers[CPSR].M0=0;
    std::cout<<"After cambio"<<(uint)registers[CPSR].word<<std::endl;*/
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
 * @brief On entry:
 * 1a - Preserves the address of the next instruction in the appropriate
 *     Link Register, next instruction is specified by the offset parameter
 * \n
 * 2a - Copies CPSR into the appropriate SPSR
 * 3a - Forces the CPSR mode bits to a value which depends on the exception
 * 4a - Forces the PC to fetch the next instruction from the relevant exception vector
 * Entering from THUMB state forces a switch into ARM state when PC is loeaded 
 * with the exception vector address
 * 
 * On completion:
 * 1b - Moves the Link Register, minus an offset where appropriate, to the PC
 * 2b - Copies SPSR back to CPSR
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
/*FIXME: Shouldn't carry bit be computed according to the barrel shifter? */
/**
 * @brief sets/clears Zero (Z), Negative (N), Overflow (V) and Carry (C) flag 
 * 
 * @param Rd destination register (result of sum/sub)
 * @param Rn first operand
 * @param op2 second operand
 */
void Arm7tdmi::set_condition_code_flags(int32_t Rd, int32_t Rn, int32_t op2)
{
    registers[get_register(CPSR)].Z = (Rd == 0) ? 1 : 0;
    registers[get_register(CPSR)].N = (Rd < 0)  ? 1 : 0;

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
  Aka, should the mode be got in the SPSR_svc register if i am in the supervisor register?*/
Arm7tdmi::_access_mode Arm7tdmi::get_access_mode(){
    return (_access_mode)(registers[CPSR].word & 0xF);
}

/**
 * @brief based on [I] flag, assigns second operand either
 * to an immediate value (I set) or to the content of the register
 * pointed by op2. Shifts the result as specified by the instruction
 * 
 * @param type type of shift performed
 * @param ins instruction
 * @return second operand  
 */
int32_t Arm7tdmi::get_ALU_op2(_shift type, _instruction ins)
{
    uint8_t shift;
    int32_t op2;
    
    if ((ins.opcode_id2 & 0x2) == 0x2) // bit[25] aka I flag is set
    { 
        op2 = ins.word & 0xFF;
        shift = (ins.word & 0xF00) >> 7; // double the "rotate" field value
        op2 = ((op2 >> shift) | (op2 << (32 - shift)));
    }
    else // No immediate operand(bit[25]/I flag unset)
    {
        op2 = registers[get_register((_registers)(ins.Rm))].word;
        shift = (ins.word & 0x10) ? 
                (ins.word >> 8) & 0xF : // [4] set -> shift amount specified by the bottom byte of Rs
                (ins.word >> 7) & 0x1F; // [4] clear -> shift amount is a 5 bit unsigned integer
        op2 = type == LL ? (op2 << shift) :
              type == LR ? ((uint32_t)op2 >> shift) :
              type == AR ? (op2 >> shift) :
              /* shift == RR? */ ((op2 >> shift) | (op2 << (32 - shift)));
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
    case AL: return true;

    default: return true; // warning line that never runs
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
 * @brief Wrapper for decode_executeTHUMB and decode_executeARM32
 * 
 * @param ins Instruction to be decoded and executed. It can be decoded either in ARM mode or in THUMB mode
 */
void Arm7tdmi::decode_execute(Arm7tdmi::_instruction ins)
{
    // Undef when [25:27] -> 011 && [4] set
    if((ins.word & 0x0E000010) == 0x06000010)
    {
        undef(ins);
        return;
    }

    if(get_mode() == THUMB_MODE) 
        decode_executeTHUMB(ins);
    else
        decode_executeARM32(ins);
}
/*Decoding a single THUMB instruction*/
void Arm7tdmi::decode_executeTHUMB(Arm7tdmi::_instruction ins)
{
    uint8_t hi_byte = ins.halfword_lo >> 8; // isolate hi byte

    switch(hi_byte >> 4) // isolate higher 4 bits to make it more readable (bits[12:15])
    {
    case 0x0: {
        switch(hi_byte)//bits[8:15]
        {
        case 0x00: case 0x01: case 0x02: case 0x03: 
        case 0x04: case 0x05: case 0x06: case 0x07: LSL(ins); break; // 0000 0xxx
        case 0x08: case 0x09: case 0x0C: case 0x0D:
        case 0x0A: case 0x0B: case 0x0E: case 0x0F: LSR(ins); break; // 0000 1xxx
        }
        break;
    }
    case 0x1: {
        switch(hi_byte)//bits[8:15]
        {
        case 0x10: case 0x11: case 0x12: case 0x13: 
        case 0x14: case 0x15: case 0x16: case 0x17: ASR(ins);  break; // 0001 0xxx
        case 0x18: case 0x19: case 0x1C: case 0x1D: ADD(ins); break; // 0001 1x0x
        case 0x1A: case 0x1B: case 0x1E: case 0x1F: SUB(ins); break; // 0001 1x1x
        }
        break;
    }
    case 0x2: {
        switch(hi_byte)
        {
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27: MOV(ins); break; // 0010 0xxx
        case 0x28: case 0x29: case 0x2C: case 0x2D:
        case 0x2A: case 0x2B: case 0x2E: case 0x2F: CMP(ins); break;  // 0010 1xxx
        }
        break;
    }
    case 0x3: {
        switch(hi_byte)
        {
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27: ADD(ins); break; // 0011 0xxx
        case 0x28: case 0x29: case 0x2C: case 0x2D:
        case 0x2A: case 0x2B: case 0x2E: case 0x2F: SUB(ins); break; // 0011 1xxx
        }
        break;
    }
    case 0x4: {
        switch((hi_byte & 0x0C))
        {
        case 0x0: { // ALU operations
            switch((ins.halfword_lo & 0x03C0) >> 6) // isolate opcode
            {
            case 0x0: AND(ins); break;
            case 0x1: EOR(ins); break;
            case 0x2: LSL(ins); break;
            case 0x3: LSR(ins); break;
            case 0x4: ASR(ins); break;
            case 0x5: ADC(ins); break;
            case 0x6: SBC(ins); break;
            case 0x7: ROR(ins); break;
            case 0x8: TST(ins); break;
            case 0x9: NEG(ins); break;
            case 0xA: CMP(ins); break;
            case 0xB: CMN(ins); break;
            case 0xC: ORR(ins); break;
            case 0xD: MUL(ins); break;
            case 0xE: BIC(ins); break;
            case 0xF: MVN(ins); break;
            }
        }  
        case 0x1: { // High Register operations/Branch Exchange
            switch(hi_byte & 0x03) // isolate opcode
            {
            case 0x0: ADD(ins); break;
            case 0x1: CMP(ins); break;
            case 0x2: MOV(ins); break;
            case 0x3:
                if (ins.halfword_lo & 0x0080) // H0[0]
                    BX(ins);
                else
                    undef(ins);
                break;
            }
        }
        default: LDR(ins); // PC-relative Load
        }
        break;    
    }
    case 0x5: {
        switch(hi_byte & 0x0E) // isolate [9:11]
        {   // Load/Store with Register Offset
        case 0x0: STR(ins); break;
        case 0x2: STRB(ins); break;
        case 0x4: LDR(ins); break;
        case 0x6: LDRB(ins); break;
            // Load/Store Sign-Extended Byte/Halfword
        case 0x1: STRH(ins); break;
        case 0x3: LDRH(ins); break;
        case 0x5: LDRSB(ins); break; // technically should be LDSB but it seems completely equivalent to ARM ::= LDSRB
        case 0x7: LDRSH(ins); break; // same as above
        }
        break;
    }
    case 0x6: case 0x7: { // Load/Store with Immediate Offset
        switch(hi_byte & 0x18) // isolate B and L flags
        {
        case 0x00: STR(ins); break;
        case 0x08: LDR(ins); break;
        case 0x10: STRB(ins); break;
        case 0x18: LDRB(ins); break;
        }
        break;
    }
    case 0x8: // Load/Store Halfword
        if ((hi_byte & 0x08) == 0) // L[0]
            STRH(ins); 
        else 
            LDRH(ins); 
        break;  
    case 0x9: // SP-relative Load/Store
        if ((hi_byte & 0x08) == 0) // L[0]
            STR(ins); 
        else 
            LDR(ins); 
        break;  
    case 0xA: ADD(ins); break; // Load Address
    case 0xB: {
        switch(hi_byte)
        {   // Add Offset to Stack Pointer
        case 0xB0: ADD(ins); break;
        case 0xB1: SUB(ins); break;
            // Push/Pop Registers
        case 0xB4: case 0xB5: PUSH(ins); break;
        case 0xBA: case 0xBB: POP(ins); break;
        default: undef(ins);
        }
        break;
    }
    case 0xC: // Multiple Load/Store
        if ((hi_byte & 0x08) == 0) // L[0]
            STMIA(ins);
        else
            LDMIA(ins);
        break;
    case 0xD:
        switch(hi_byte & 0x0F) // cond
        {
        case 0x0F: SWI(ins); break;
        case 0x0E: undef(ins); break;
        default: B(ins, (_cond)(hi_byte & 0x0F));
        }
        break;
    case 0xE:
        if((hi_byte & 0x08) == 0x00)
            B(ins, AL);
        else
            undef(ins);
        break;
    case 0xF:
        if((hi_byte & 0x08) == 0x00) // H[0]
            BL(ins);
        else // ??? not specified ffs, I assume BL
            BL(ins);
        break;
    }
    return;
}
/*Decoding a single ARM32 instruction*/
void Arm7tdmi::decode_executeARM32(Arm7tdmi::_instruction ins)
{
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
            case 0x0: ANDS(ins); break;
            case 0x1: EORS(ins); break;
            case 0x2: SUBS(ins); break;
            case 0x3: RSB(ins); break;
            case 0x4: ADDS(ins); break;
            case 0x5: ADCS(ins); break;
            case 0x6: SBCS(ins); break;
            case 0x7: RSC(ins); break;
            case 0x8: {
                    if (ins.opcode_id2 & 0x1) // S[1]
                        TST(ins);
                    else if ((ins.word & 0x020F0FFF) == 0x000F0000) // I[0] && SBO [16:19] && SBZ [0:11]
                        MRS(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0x9: {
                    if (ins.opcode_id2 & 0x1) // S[1]
                        TEQ(ins);
                    else if ((ins.word & 0x0200F000) == 0x0200F000) // I[1] && SBO [12:15]
                        MSR(ins);
                    else if ((ins.word & 0x0200FFF0) == 0x0000F000) // I[0] && SBO [11:15] && SBZ [4:11]
                        MSR(ins);
                    else if ((ins.word & 0x020FFF10) == 0x000FFF10) // I[0] && SBO [8:19] && [4] set
                        BX(ins);
                    else
                        undef(ins);
                    break; 
                }
            case 0xA: {
                    if ((ins.word & 0x0010F000) == 0x00100000) // S[1] && SBZ [12:15]
                        CMP(ins);
                    else if ((ins.word & 0x020F0FFF) == 0x000F0000) // I[0] && SBO [16:19] && SBZ [0:11]
                        MRS(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0xB: {
                    if ((ins.word & 0x0010F000) == 0x00100000) // S[1] && SBZ [12:15]
                        CMN(ins);
                    else if ((ins.word & 0x0200F000) == 0x0200F000) // I[1] && SBO [12:15]
                        MSR(ins);
                    else if ((ins.word & 0x0200FFF0) == 0x0000F000) // I[0] && SBO [11:15] && SBZ [4:11]
                        MSR(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0xC: ORR(ins); break;
            case 0xD: {
                    if ((ins.word & 0xF0000) == 0x00000) // SBZ [16:19]
                        MOVS(ins);
                    else
                        undef(ins);
                    break;
                }
            case 0xE: BICS(ins); break;
            case 0xF: {
                    if ((ins.word & 0xF0000) == 0x00000) // SBZ [16:19]
                        MVNS(ins);
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
            case 0x0: case 0x1: MULS(ins); break;
            case 0x2: case 0x3: MLA(ins); break;
                // Multiply Long
            case 0x8: case 0x9: UMULL(ins); break;
            case 0xA: case 0xB: UMLAL(ins); break;
            case 0xC: case 0xD: SMULL(ins); break;
            case 0xE: case 0xF: SMLAL(ins); break;
                // Single Data Swap
            case 0x10: SWP(ins); break;
            case 0x14: SWPB(ins); break;
            }
            break;

        case 0xB:
            if((ins.opcode_id2 & 1) == 1)
                LDRH(ins);
            else
                STRH(ins);
            break;

        case 0xD:
            if((ins.opcode_id2 & 1) == 1)
                LDRSB(ins);
            else
                undef(ins);
            break;

        case 0xF:
            if((ins.opcode_id2 & 1) == 1)
                LDRSH(ins);
            else
                undef(ins);
            break;
        }
        break;
    }

    case 0x1: {
        switch(ins.opcode_id2 & 0x5) // isolate bits 20 and 22
        {
        case 0x0: STR(ins); break;  // xxxx x0x0
        case 0x1: LDR(ins); break;  // xxxx x0x1
        case 0x4: STRB(ins); break; // xxxx x1x0
        case 0x5: LDRB(ins); break; // xxxx x1x1
        }
        break;
    }

    case 0x2: {
        switch(ins.opcode_id2 & 0x21) // isolate bits 20 and 25
        {
        case 0x20: case 0x21: B(ins, (_cond)(ins.word >> 28)); break; // xx1x xxx0 U xx1x xxx1
        case 0x01: LDM(ins); break; // xx0x xxx1
        case 0x00: STM(ins); break; // xx0x xxx0
        }
        break;
    }

    case 0x3: {   
        if(ins.opcode_id2 & 0x30)
            SWI(ins);
        else 
            undef(ins);
        break;
    }
    }
}