/* 
 * File:   alu.h
 * Group: Dai-gurran
 *
 * Created on January 21, 2013, 1:17 PM
 */

#ifndef ALU_H
#define	ALU_H
#include "global.h"
#include "register.h"


typedef struct {
    Register A;
    Register B;
    Register R;
    Register R2;
    StatusWord *SW;
    Boolean a_set; //Has the a operand been set since the last operation?
    Boolean b_set; //Has the b operand been set since the last operation?
    Boolean r_set; //Has the result been set since the last operation?
    Boolean r2_set; //Has the result2 been set since the last operation?
} ALU;

/*Initializes the ALU.*/
char* alu_init(ALU* this, StatusWord* SW);
/*Sets both operand registers.*/
char* alu_set_rs(ALU* this, Register rx, Register ry);
/*Sets rx operand register.*/
char* alu_set_rx(ALU* this, Register rx);
/*Sets ry operand register.*/
char* alu_set_ry(ALU* this, Register ry);
/*Retrieves the ALU result. Call after calling an operation function.*/
char* alu_get_res(ALU* this, Register* res);
/*Retrieves the ALU result2. Call after calling an operation function.*/
char* alu_get_res2(ALU* this, Register* res2);

/*ALU Operations. All will fail if the appropriate operands have not been set.*/
/*Two Operand Instructions*/
char* alu_op_add(ALU* this);
char* alu_op_sub(ALU* this);
char* alu_op_mul(ALU* this);
char* alu_op_div(ALU* this);
char* alu_op_and(ALU* this);
char* alu_op_or(ALU* this);
char* alu_op_xor(ALU* this);
/*One Operand Instructions*/
char* alu_op_not(ALU* this);
char* alu_op_shl(ALU* this);
char* alu_op_shr(ALU* this);
#endif	/* ALU_H */
