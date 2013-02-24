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


typedef struct {
    //structures
    ALU alu;
    RegisterFile rf;
    
    //special registers
    Register SW;
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
    cpu_op_code_parts parts:5;
    unsigned char full:5;
} cpu_op_code ;

//basic instruction
typedef struct {
    unsigned short everything_else:11;
    cpu_op_code opcode:5;
} cpu_instruction;



//two word instruction
typedef struct {
    unsigned short zeros:7;
    unsigned short d:4;
    cpu_op_code opcode:5;
} cpu_inst_formatw2;

//two word instruction
typedef struct {
    unsigned short word2;
    cpu_inst_formatw2 word1;
} cpu_inst_format0;

typedef struct {
    unsigned short immed7:7;
    unsigned short d:4;
    cpu_op_code opcode:5;
} cpu_inst_format1;

typedef struct {
    unsigned short mod:3;
    unsigned short a:4;
    unsigned short d:4;
    cpu_op_code opcode:5;
} cpu_inst_format2;

typedef struct {
    unsigned short immed11:11;
    cpu_op_code opcode:5;
} cpu_inst_format3;

typedef struct {
    unsigned short c:2;
    unsigned short immed9:9;
    cpu_op_code opcode:5;
} cpu_inst_format4;

typedef struct {
    unsigned char c:2;
    unsigned char zeros:5;
    unsigned char r:4;
    cpu_op_code opcode:5;
} cpu_inst_format5;

typedef struct {
    unsigned short zeros:5;
    unsigned short v:6;
    cpu_op_code opcode:5;
} cpu_inst_format6;

typedef struct {
    unsigned short m:1;
    unsigned short zeros:9;
    cpu_op_code opcode:5;
} cpu_inst_format7;

typedef struct {
    unsigned short m:1;
    unsigned short p:6;
    unsigned short d:4;
    cpu_op_code opcode:5;
} cpu_inst_format8;

typedef struct {
    unsigned short mod:3;
    unsigned short q:8;
    cpu_op_code opcode:5;
} cpu_inst_format9;



#endif	/* CPU_H */

//