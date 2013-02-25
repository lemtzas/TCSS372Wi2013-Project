/* 
 * File:   cpu.h
 * Author: David, Lawrence
 *
 * Created on February 16, 2013, 1:16 PM
 */

#ifndef CPU_H
#define	CPU_H

#include "alu.h"
#include "registerFile.h"
#include "register.h"
#include "memory.h"

#define IR_START        0x0000
#define MAR_START       0x0000
#define MDR_START       0x0000
#define OSB_START       0x0000
#define TXB_START       0x0000
#define PC_START        0x3000
#define SW_START        0x4000 //0b0100 0000 0000 0000 (Z)

#define REG_BP          0xE
#define REG_IP_0        0xC
#define REG_IP_1        0xD
#define REG_SP          0xF
#define SP_START        0xFFFF

#define REG_RES         0x0
#define REG_RES2        0x8

#define OP1_LDST        0
#define OP1_ALU         1
#define OP1_CONTROL     2
#define OP1_SUPMISC     3

#define OP2_LDI         1
#define OP2_LDA         2
#define OP2_LDB         3
#define OP2_LDWMOV      4
#define OP2_STB         5
#define OP2_STW         6
#define OP2_STACK       7

#define OP2_OPER        1
#define OP2_SHL         0
#define OP2_SHR         2

#define OP2_IRQ8        1
#define OP2_HALT        2
#define OP2_NOP         3
#define OP2_LDOS        4
#define OP2_LDRO        5

#define OP2_BR_imm11     0
#define OP2_BR_reg       1
#define OP2_BRnzco_imm9  2
#define OP2_BRnzco_reg   3
#define OP2_JSR_reg      4
#define OP2_JSR_nzco_reg 5
#define OP2_TRAP         6
#define OP2_RET          7

#define MOD_CC_N         0
#define MOD_CC_Z         1
#define MOD_CC_C         2
#define MOD_CC_O         3

#define MOD_MODE_REGISTER       0
#define MOD_MODE_RELATIVE       1
#define MOD_MODE_INDEX          2
#define MOD_MODE_INDIRECT       3
#define MOD_MODE_MOV            4

#define MOD_STACK_PUSHB         1
#define MOD_STACK_POPB          2
#define MOD_STACK_PUSHW         3
#define MOD_STACK_POPW          4

#define MOD_OPER_ADD            0
#define MOD_OPER_SUB            1
#define MOD_OPER_MUL            2
#define MOD_OPER_DIV            3
#define MOD_OPER_AND            4
#define MOD_OPER_OR             5
#define MOD_OPER_XOR            6
#define MOD_OPER_NOT            7

typedef struct {
    //structures
    ALU alu;
    RegisterFile rf;
    Memory memory;
    
    //special registers
    StatusWord SW;
    Register IR;
    Register PC;
    Register OSB;
    Register TXB;
    Register MAR;
    Register MDR;
    
    unsigned char halt;
} CPU;


char* cpu_init  (CPU *cpu);
char* cpu_step  (CPU *cpu);
char* cpu_run   (CPU *cpu);


//remember to put fields in reverse
typedef struct {
    unsigned char op2:3;
    unsigned char op1:2;
} cpu_op_code_parts;

typedef union {
    cpu_op_code_parts parts;
    unsigned char full:5;
} cpu_op_code ;

//basic instruction
typedef struct {
    uint16_t everything_else:11;
    cpu_op_code opcode;
} cpu_instruction;

//two word instruction
typedef struct {
    uint16_t zeros:7;
    uint16_t d:4;
    cpu_op_code opcode;
} cpu_inst_formatw2;

//two word instruction
typedef struct {
    //uint16_t word2;
    cpu_inst_formatw2 word1;
} cpu_inst_format0;

typedef struct {
    uint16_t immed7:7;
    uint16_t d:4;
    cpu_op_code opcode;
} cpu_inst_format1;

typedef struct {
    uint16_t mod:3;
    uint16_t a:4;
    uint16_t d:4;
    cpu_op_code opcode;
} cpu_inst_format2;

typedef struct {
    uint16_t immed11:11;
    cpu_op_code opcode;
} cpu_inst_format3;

typedef struct {
    uint16_t c:2;
    uint16_t immed9:9;
    cpu_op_code opcode;
} cpu_inst_format4;

typedef struct {
    unsigned char c:2;
    unsigned char zeros:5;
    unsigned char r:4;
    cpu_op_code opcode;
} cpu_inst_format5;

typedef struct {
    uint16_t zeros:5;
    uint16_t v:6;
    cpu_op_code opcode;
} cpu_inst_format6;

typedef struct {
    uint16_t m:1;
    uint16_t zeros:9;
    cpu_op_code opcode;
} cpu_inst_format7;

typedef struct {
    uint16_t m:1;
    uint16_t p:6;
    uint16_t d:4;
    cpu_op_code opcode;
} cpu_inst_format8;

typedef struct {
    uint16_t mod:3;
    uint16_t q:8;
    cpu_op_code opcode;
} cpu_inst_format9;


char* cpu_getword(CPU *cpu);
char* cpu_inst_fetch(CPU *cpu);
char* cpu_inst_decode(CPU *cpu);

char* cpu_inst_LDST(CPU *cpu);
char* cpu_inst_LDST_LDI(CPU *cpu);
char* cpu_inst_LDST_LDA(CPU *cpu);
char* cpu_inst_LDST_LDB(CPU *cpu);
char* cpu_inst_LDST_LDWMOV(CPU *cpu);
char* cpu_inst_LDST_STB(CPU *cpu);
char* cpu_inst_LDST_STW(CPU *cpu);
char* cpu_inst_LDST_STACK(CPU *cpu);

char* cpu_inst_ALU(CPU *cpu);
char* cpu_inst_ALU_OPER(CPU *cpu);
char* cpu_inst_ALU_SHLR(CPU *cpu);

char* cpu_inst_CONTROL(CPU *cpu);
char* cpu_inst_CONTROL_BR_imm11(CPU *cpu);
char* cpu_inst_CONTROL_BR_reg(CPU *cpu);
char* cpu_inst_CONTROL_BRnzco_imm9(CPU *cpu);
char* cpu_inst_CONTROL_BRnzco_reg(CPU *cpu);
char* cpu_inst_CONTROL_JSR_reg(CPU *cpu);
char* cpu_inst_CONTROL_JSR_nzco_reg(CPU *cpu);
char* cpu_inst_CONTROL_TRAP(CPU *cpu);
char* cpu_inst_CONTROL_RET(CPU *cpu);

char* cpu_inst_SUPMISC(CPU *cpu);

Word sext(Word word, int bits);

#endif	/* CPU_H */


//
