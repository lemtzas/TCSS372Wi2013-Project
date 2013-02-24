/* 
 * File:   cpu.h
 * Author: David
 *
 * Created on February 16, 2013, 1:16 PM
 */

#ifndef CPU_H
#define	CPU_H

#include "alu.h"
#include "registerFile.h"
#include "register.h"
#include "memory.h"

#define REG_BP          0xE
#define REG_IP_0        0xC
#define REG_IP_1        0xD
#define REG_SP          0xF
#define SP_START        0xFFFF

#define REG_RES         0x0
#define REG_RES2        0x8

#define OP1_LDST        0b00
#define OP1_ALU         0b01
#define OP1_CONTROL     0b10
#define OP1_SUPMISC     0b11

#define OP2_LDI         0b001
#define OP2_LDA         0b010
#define OP2_LDB         0b011
#define OP2_LDWMOV      0b100
#define OP2_STB         0b101
#define OP2_STW         0b110
#define OP2_STACK       0b111

#define OP2_OPER        0b001
#define OP2_SHL         0b000
#define OP2_SHR         0b010

#define OP2_IRQ8        0b001
#define OP2_HALT        0b010
#define OP2_NOP         0b011
#define OP2_LDOS        0b100
#define OP2_LDRO        0b101

#define OP2_BR_imm11     0b000
#define OP2_BR_reg       0b001
#define OP2_BRnzco_imm9  0b010
#define OP2_BRnzco_reg   0b011
#define OP2_JSR_reg      0b100
#define OP2_JSR_nzco_reg 0b101
#define OP2_TRAP         0b110
#define OP2_RET          0b111

#define MOD_CC_N         0b00
#define MOD_CC_Z         0b01
#define MOD_CC_C         0b10
#define MOD_CC_O         0b11

#define MOD_MODE_REGISTER       0b000
#define MOD_MODE_RELATIVE       0b001
#define MOD_MODE_INDEX          0b010
#define MOD_MODE_INDIRECT       0b011
#define MOD_MODE_MOV            0b100

#define MOD_STACK_PUSHB         0b001
#define MOD_STACK_POPB          0b010
#define MOD_STACK_PUSHW         0b011
#define MOD_STACK_POPW          0b100

#define MOD_OPER_ADD            0b000
#define MOD_OPER_SUB            0b001
#define MOD_OPER_MUL            0b010
#define MOD_OPER_DIV            0b011
#define MOD_OPER_AND            0b100
#define MOD_OPER_OR             0b101
#define MOD_OPER_XOR            0b110
#define MOD_OPER_NOT            0b111

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