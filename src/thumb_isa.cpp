#include "arm7tdmi.hh"
using namespace cpu;
/*I metodi come ADDS, ADCS, EORS, ANDS etc sono l'equivalente ARM dei metodi ADD, ADC etc in THUMB
  Tuttavia questi(ADCS, ANDS etc) non sono metodi diversi ma bensì gli stessi metodi ADD,ADC in ARM con il flag S settato
  TODO: Dunque bisogna eliminarli  */
//Rd = Rd + Rs + C-bit (THUMB)
/*ALU THUMB Opreations*/
void Arm7tdmi::AND(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word&rn.word;
}
void Arm7tdmi::ADC(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//Rn is Rs in THUMB
    rd.word=(rd.word+rn.word)+registers[CPSR].C;//Not sure if cpu should perform ALU operations in 32 bit
}
void Arm7tdmi::BIC(Arm7tdmi::_instruction instruction){
     _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word&~rn.word;
}
void Arm7tdmi::MVN(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=~rn.word;
}
void Arm7tdmi::ORR(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word|rn.word;
    
}
void Arm7tdmi::MUL(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word*rn.word;
    }
void Arm7tdmi::EOR(Arm7tdmi::_instruction instruction){
    _register_type rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word^rn.word;
}
void Arm7tdmi::LSL(Arm7tdmi::_instruction instruction){
    _register_type rd,rn;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word<<rn.word;
} 
void Arm7tdmi::LSR(Arm7tdmi::_instruction instruction){
    _register_type rd,rn;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word>>rn.word;
}
void Arm7tdmi::NEG(Arm7tdmi::_instruction instruction){
    _register_type rd,rn;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=0-rn.word;
}
/*In C/C++, right shift is a logical one when dealing with unsigned,while it behaves like an arithmetic one when dealing with signed
  Thus the code is the same as LSR(might as well calling directly the LSR method)   */
void Arm7tdmi::ASR(Arm7tdmi::_instruction instruction){
    _register_type rd,rn;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word>>rn.word;
}

void Arm7tdmi::ROR(Arm7tdmi::_instruction instruction){
    //https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
    _register_type rd,rn;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=(rd.word >> rn.word)|(rd.word << (32 - rn.word));
} 
void Arm7tdmi::SUB(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SBC(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::PUSH(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::POP(Arm7tdmi::_instruction instruction){}
//void Arm7tdmi::STMIA(Arm7tdmi::_instruction instruction){} 
//void Arm7tdmi::LDMIA(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::BL(Arm7tdmi::_instruction instruction){}