#include "arm7tdmi.hh"
using namespace cpu;
void Arm7tdmi::ADDS(Arm7tdmi::_instruction instruction){
   
}
/*
  Rd = Rn + Op2+ C-bit (ARM32) */
void Arm7tdmi::ADCS(Arm7tdmi::_instruction instruction){
    _register rd,rn,op2;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/
    
}
void Arm7tdmi::ANDS(Arm7tdmi::_instruction instruction){
    _register rd,rn,op2;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/
}
void Arm7tdmi::BICS(Arm7tdmi::_instruction instruction){
     _register rd,rn,op2;
    rd=get_register((_registers)(instruction.halfword_lo&0xF));
    rn=get_register((_registers)((instruction.halfword_lo>>4)&0xF));//rn is rs in THUMB
    rd.word=rd.word&~rn.word;

}
void Arm7tdmi::EORS(Arm7tdmi::_instruction instruction){
    _register rd,rn,op2;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/
}
void Arm7tdmi::MULS(Arm7tdmi::_instruction instruction){
    _register rd,rn,op2;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/ 
}
void Arm7tdmi::MVNS(Arm7tdmi::_instruction instruction){
    _register rd,rn,op2;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/
}
void Arm7tdmi::ORRS(Arm7tdmi::_instruction instruction){
    _register rd,rn,op2;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
            /*Calcolo dell'operand 2 che non ho voglia di fare perchè mi hai detto di fare prima la thumb xd*/
}
void Arm7tdmi::SUBS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SBCS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::B(Arm7tdmi::_instruction instruction, _cond condition){}
void Arm7tdmi::BX(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::CMN(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::CMP(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::LDM(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::LDR(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::LDRB(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::LDRH(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::LDRSB(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::LDRSH(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::MLA(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::MOVS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::MRS(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::MSR(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::RSB(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::RSC(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SMLAL(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SMULL(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::STM(Arm7tdmi::_instruction instruction){} 
void Arm7tdmi::STR(Arm7tdmi::_instruction instruction){} 
void Arm7tdmi::STRB(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::STRH(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SWI(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SWP(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::SWPB(Arm7tdmi::_instruction instruction){}
void Arm7tdmi::TEQ(Arm7tdmi::_instruction instruction){} 
void Arm7tdmi::TST(Arm7tdmi::_instruction instruction){} 
void Arm7tdmi::UMLAL(Arm7tdmi::_instruction instruction){} 
void Arm7tdmi::UMULL(Arm7tdmi::_instruction instruction){} 

