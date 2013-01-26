#include "alu.h"

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
}

/*Sets both operand registers.*/
char* alu_set_rs(ALU* this, Register rx, Register ry)
{
    if(!this) return "[alu_set_rs] Must provide ALU!";
    this->A = rx;
    this->B = ry;
    this->a_set = TRUE;
    this->b_set = TRUE;
    return 0;
}

/*Sets rx operand register.*/
char* alu_set_rx(ALU* this, Register rx)
{
    if(!this) return "[alu_set_rs] Must provide ALU!";
    this->A = rx;
    this->a_set = TRUE;
    return 0;
}

/*Sets ry operand register.*/
char* alu_set_ry(ALU* this, Register ry)
{
    if(!this) return "[alu_set_rs] Must provide ALU!";
    this->B = ry;
    this->b_set = TRUE;
    return 0;
}

/*Retrieves the ALU result. Call after calling a /single/ operation function.*/
char* alu_get_res(ALU* this, Register* res)
{
    if(!this) return "[alu_get_res] Must provide ALU!";
    if(!this->r_set) return "[alu_get_res] Must perform an operation!";
    (*res) = this->R;
    return 0;
    
}

/*Retrieves the ALU result. Call after calling a /single/ operation function.*/
char* alu_get_res(ALU* this, Register* res)
{
    
}

/*ALU Operations. All will fail if the appropriate operands have not been set.*/
/*Two Operand Instructions*/
char* alu_op_add(ALU* this)
{
    
}

char* alu_op_sub(ALU* this)
{
    
}

char* alu_op_mul(ALU* this)
{
    
}

char* alu_op_div(ALU* this)
{
    
}

char* alu_op_and(ALU* this)
{
    
}

char* alu_op_or(ALU* this)
{
    
}

char* alu_op_xor(ALU* this)
{
    
}

char* alu_op_not(ALU* this)
{
    
}
/*One Operand Instructions*/
char* alu_op_not(ALU* this)
{
    
}

char* alu_op_shl(ALU* this)
{
    
}

char* alu_op_shr(ALU* this)
{
    
}