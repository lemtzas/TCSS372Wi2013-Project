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
#include "read_char.h"

//default values
#define IR_START        0x0000
#define MAR_START       0x0000
#define MDR_START       0x0000
#define OSB_START       0x0000
#define TXB_START       0x0000
#define PC_START        0x3000
#define SW_START        0x4000 //0b0100 0000 0000 0000 (Z)

//convenience values
#define REG_BP          0xE
#define REG_IP_0        0xC
#define REG_IP_1        0xD
#define REG_SP          0xF
#define SP_START        0xFFFF

//return registers
#define REG_RES         0x0
#define REG_RES2        0x8

//major execute categories
#define OP1_LDST        0
#define OP1_ALU         1
#define OP1_CONTROL     2
#define OP1_SUPMISC     3

//more specific execute instructions
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

#define OP2_INOUT       0
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

//mode modifers
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

#define MOD_INOUT_IN            0
#define MOD_INOUT_OUT           1

#define IOPORT_VID              0x30
#define IOPORT_KBD              0x38

#define IOPORT_MEM_SIZE         0xff

typedef struct {
    //structures
    ALU alu;
    RegisterFile rf;
    Memory memory;
    Byte IOPORT[IOPORT_MEM_SIZE];
    
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

//initialize the cpu and its substructures
char* cpu_init  (CPU *cpu);
//runs the controller through a single fetch-decode-execute cycle
char* cpu_step  (CPU *cpu);
//char* cpu_run   (CPU *cpu);
char* check_keyboard (CPU *cpu);
char* do_output (CPU *cpu);




//the following operations simulate the Mem[MAR] -> MDR behavior
char* cpu_getword(CPU *cpu);
char* cpu_setword(CPU *cpu);
char* cpu_getbyte(CPU *cpu);
char* cpu_setbyte(CPU *cpu);

//sign extend utility
Word sext(Word word, int bits);

//fetch macrostate
char* cpu_inst_fetch(CPU *cpu);
//decode macrostate, calls the appropriate "execute" macrostate
char* cpu_inst_decode(CPU *cpu);

//LD/ST execute macrostate, calls execute for specific instruction below
char* cpu_inst_LDST(CPU *cpu);
char* cpu_inst_LDST_LDI(CPU *cpu);
char* cpu_inst_LDST_LDA(CPU *cpu);
char* cpu_inst_LDST_LDB(CPU *cpu);
char* cpu_inst_LDST_LDWMOV(CPU *cpu);
char* cpu_inst_LDST_STB(CPU *cpu);
char* cpu_inst_LDST_STW(CPU *cpu);
char* cpu_inst_LDST_STACK(CPU *cpu);

//ADD execute macrostate, calls execute for specific instruction below
char* cpu_inst_ALU(CPU *cpu);
char* cpu_inst_ALU_OPER(CPU *cpu);
char* cpu_inst_ALU_SHLR(CPU *cpu);

//CONTROL execute macrostate, calls execute for specific instruction below
char* cpu_inst_CONTROL(CPU *cpu);
char* cpu_inst_CONTROL_BR_imm11(CPU *cpu);
char* cpu_inst_CONTROL_BR_reg(CPU *cpu);
char* cpu_inst_CONTROL_BRnzco_imm9(CPU *cpu);
char* cpu_inst_CONTROL_BRnzco_reg(CPU *cpu);
char* cpu_inst_CONTROL_JSR_reg(CPU *cpu);
char* cpu_inst_CONTROL_JSR_nzco_reg(CPU *cpu);
char* cpu_inst_CONTROL_TRAP(CPU *cpu);
char* cpu_inst_CONTROL_RET(CPU *cpu);

//SUPERVISOR and MISC execute macrostate, calls execute for specific instruction below
char* cpu_inst_SUPMISC(CPU *cpu);
char* cpu_inst_SUPMISC_INOUT(CPU *cpu);


//remember to put fields in reverse
//basic instruction
typedef struct {
    uint16_t everything_else:11;
    uint8_t op2:3;uint8_t op1:2;
} cpu_instruction;

//format0 instruction struct, for casting
typedef struct {
    //uint16_t word2;
    uint16_t zeros:7;
    uint16_t d:4;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format0;

//format1 instruction struct, for casting
typedef struct {
    uint16_t immed7:7;
    uint16_t d:4;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format1;

//format2 instruction struct, for casting
typedef struct {
    uint16_t mod:3;
    uint16_t a:4;
    uint16_t d:4;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format2;

//format3 instruction struct, for casting
typedef struct {
    uint16_t immed11:11;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format3;

//format4 instruction struct, for casting
typedef struct {
    uint16_t c:2;
    uint16_t immed9:9;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format4;

//format5 instruction struct, for casting
typedef struct {
    unsigned char c:2;
    unsigned char zeros:5;
    unsigned char r:4;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format5;

//format6 instruction struct, for casting
typedef struct {
    uint16_t zeros:5;
    uint16_t v:6;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format6;

//format7 instruction struct, for casting
typedef struct {
    uint16_t m:1;
    uint16_t zeros:9;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format7;

//format8 instruction struct, for casting
typedef struct {
    uint16_t m:1;
    uint16_t p:6;
    uint16_t d:4;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format8;

//format9 instruction struct, for casting
typedef struct {
    uint16_t mod:3;
    uint16_t q:8;
    uint8_t op2:3;uint8_t op1:2;
} cpu_inst_format9;

#endif	/* CPU_H */


//
