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
#endif	/* CPU_H */

