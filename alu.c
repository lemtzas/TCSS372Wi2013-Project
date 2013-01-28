/*
 * File: alu.c
 * Group: Dai-Gurran
 * Author:
 */
#include "alu.h"

//TODO: Please review this for accuracy
//TODO: Overflow and Carry is only possible during ADD and SUB. Move it there
//TODO: Rename to represent altered functionality.
char* _alu_update_SW(ALU* this)
{
    //clear NZCO
    (*(this->SW)) &= 0x0FFF;
    //[N] set negative bit
    (*(this->SW)) |= (0x8000 & this->R);
    //[Z] set zero bit
    if(!this->R) 
        (*(this->SW)) |= 0x4000;
    //[C] set carry bit (detects unsigned overflow)
    if( ((unsigned int)this->A+(unsigned int)this->B) > (this->R) )
        (*(this->SW)) |= 0x2000;
    //[O] set overflow bit (detects sign overflow)
    if( ((short)this->A+(short)this->B) < ((short)this->R) )
        (*(this->SW)) |= 0x1000;
}

/*Initializes the ALU.*/
char* alu_init(ALU* this, Register* SW)
{
    //init all registers
    this->A = this->B = this->R = this->R2 = 0;
    this->SW = SW;
    this->a_set = FALSE;
    this->b_set = FALSE;
    this->r_set = FALSE;
    this->r2_set = FALSE;
    return 0;
}

/*Sets both operand registers. Clears result registers.*/
char* alu_set_rs(ALU* this, Register rx, Register ry)
{
    if(!this) return "[alu_set_rs] Must provide ALU!";
    this->A = rx;
    this->B = ry;
    this->a_set = TRUE;
    this->b_set = TRUE;
    this->r_set = FALSE;
    this->r2_set = FALSE;
    return 0;
}

/*Sets rx operand register. Clears result registers.*/
char* alu_set_rx(ALU* this, Register rx)
{
    if(!this) return "[alu_set_rs] Must provide ALU!";
    this->A = rx;
    this->a_set = TRUE;
    this->r_set = FALSE;
    this->r2_set = FALSE;
    return 0;
}

/*Sets ry operand register. Clears result registers.*/
char* alu_set_ry(ALU* this, Register ry)
{
    if(!this) return "[alu_set_rs] Must provide ALU!";
    this->B = ry;
    this->b_set = TRUE;
    this->r_set = FALSE;
    this->r2_set = FALSE;
    return 0;
}

/*Retrieves the ALU result. Call after calling an operation function.*/
char* alu_get_res(ALU* this, Register* res)
{
    if(!this) return "[alu_get_res] Must provide ALU!";
    if(!this->r_set) return "[alu_get_res] Must perform an operation!";
    (*res) = this->R;
    //this->r_set = FALSE; //TODO: Do we want to clear the result?
    return 0;
    
}

/*Retrieves the ALU result2. Call after calling an operation function.*/
char* alu_get_res2(ALU* this, Register* res2)
{
    if(!this) return "[alu_get_res] Must provide ALU!";
    if(!this->r2_set) return "[alu_get_res] Must perform an operation that sets res2!";
    (*res2) = this->R2;
    //this->r2_set = FALSE; //TODO: Do we want to clear the result?
    return 0;
}

/*ALU Operations. All will fail if the appropriate operands have not been set.*/
/*Two Operand Instructions*/
char* alu_op_add(ALU* this)
{
    if(!this)           return "[alu_op_add] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_add] Must set A!";
    if(!this->b_set)    return "[alu_op_add] Must set B!";
    if(this->r_set)    return "[alu_op_add] Must reset Operands!";
    this->R = this->A + this->B;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_sub(ALU* this)
{
    if(!this)           return "[alu_op_sub] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_sub] Must set A!";
    if(!this->b_set)    return "[alu_op_sub] Must set B!";
    if(this->r_set)    return "[alu_op_sub] Must reset Operands!";
    this->R = this->A - this->B;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_mul(ALU* this)
{
    Byte a_lob,b_lob;
    char* err;
    if(!this)           return "[alu_op_mul] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_mul] Must set A!";
    if(!this->b_set)    return "[alu_op_mul] Must set B!";
    if(this->r_set)    return "[alu_op_mul] Must reset Operands!";
    err = reg_getLOB(&(this->A),&a_lob);
    if(err) return err;
    err = reg_getLOB(&(this->B),&b_lob);
    if(err) return err;
    this->R = (unsigned short)a_lob * (unsigned short)b_lob;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_div(ALU* this)
{
    if(!this)           return "[alu_op_div] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_div] Must set A!";
    if(!this->b_set)    return "[alu_op_div] Must set B!";
    if(this->r_set)    return "[alu_op_div] Must reset Operands!";
    this->R = this->A / this->B;
    this->R2 = this->A % this->B;
    this->r_set = TRUE;
    this->r2_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_and(ALU* this)
{
    if(!this)           return "[alu_op_and] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_and] Must set A!";
    if(!this->b_set)    return "[alu_op_and] Must set B!";
    if(this->r_set)    return "[alu_op_and] Must reset Operands!";
    this->R = this->A & this->B;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_or(ALU* this)
{
    if(!this)           return "[alu_op_or] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_or] Must set A!";
    if(!this->b_set)    return "[alu_op_or] Must set B!";
    if(this->r_set)    return "[alu_op_or] Must reset Operands!";
    this->R = this->A | this->B;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_xor(ALU* this)
{
    if(!this)           return "[alu_op_xor] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_xor] Must set A!";
    if(!this->b_set)    return "[alu_op_xor] Must set B!";
    if(this->r_set)    return "[alu_op_xor] Must reset Operands!";
    this->R = this->A ^ this->B;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}
/*One Operand Instructions*/
char* alu_op_not(ALU* this)
{
    if(!this)           return "[alu_op_not] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_not] Must set A!";
    if(this->r_set)    return "[alu_op_not] Must reset Operands!";
    this->R = ~this->A;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_shl(ALU* this)
{
    if(!this)           return "[alu_op_not] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_not] Must set A!";
    if(this->r_set)    return "[alu_op_not] Must reset Operands!";
    this->R = this->A << 1;
    this->r_set = TRUE;
    this->a_set = FALSE;
    this->b_set = FALSE;
    return 0;
}

char* alu_op_shr(ALU* this)
{
    if(!this)           return "[alu_op_not] Must provide ALU!";
    if(!this->a_set)    return "[alu_op_not] Must set A!";
    if(this->r_set)    return "[alu_op_not] Must reset Operands!";
    this->R = this->A >> 1;
    this->a_set = FALSE;
    this->b_set = FALSE;
    this->r_set = TRUE;
    return 0;
}


//void main(int argc, char** argv)
//{
//    ALU alu;
//    ALU sw;
//    Register r0;
//    Register a,b;
//    char* err;
//    
//    err = alu_init(&alu,&sw);
//    if(err) printf("%s\n",err);
//    err = alu_set_rs(&alu,0x0057,0x0004);
//    if(err) printf("%s\n",err);
//    err = alu_op_mul(&alu);
//    if(err) printf("%s\n",err);
//    err = alu_get_res(&alu,&r0);
//    if(err) printf("%s\n",err);
//    printf("%04x",r0);
//    return 0;
//}
