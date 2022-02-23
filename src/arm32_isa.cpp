#include "arm7tdmi.hh"
using namespace cpu;
void Arm7tdmi::ADDS(Arm7tdmi::_instruction instruction){
   _register rd,rn;
   int32_t op2;
   uint8_t shift_amount=0;
   _shift shift_type;
    rd=get_register((_registers)(instruction.Rd));
    rn=get_register((_registers)(instruction.Rn));
    shift_type=(_shift)((instruction.word>>5) &0x2);//isolating bit[5:6] to determine the type of shift
    if((instruction.opcode_id2&0x2)==0x2){//bit[25] aka I flag is set
        op2=instruction.word&0xFF;
    }else{//No immediate operand(bit[25]/I flag unset)
        op2=get_register((_registers)(instruction.Rm)).word;
        if((instruction.word&0x10)==0x10){//bit[4] set -> shift amount specified by the bottom byte of Rs
            shift_amount=(instruction.word>>8)&0xF;//isolating bit[8:11]
        }else{//bit[4] unset -> shift amount is a 5 bit unsigned integer
            shift_amount=(instruction.word>>7)&0x1F;//isolating bit[7:11]
        }
    }
    switch(shift_type){
        //Gotta handle the carry bit
        case LL:
            rd.word=rn.word+(op2<<shift_amount);
            break;
        case LR:
            rd.word=rn.word+(op2>>shift_amount);
            break;
        case AR:
            rd.word=rn.word+(op2>>shift_amount);
            break;
        case RR:
            rd.word=rn.word+(op2<<shift_amount);
            rd.word=rn.word+((op2>>shift_amount)|(op2 <<(32-shift_amount)));
            break;
    }
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

