/* 
 * File: phase1test.c
 * Group: Dai-Gurren
 * Author: Lemtzas, Lawrence Grass
 *
 * Created on January 21, 2013, 1:12 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "register.h"
#include "registerFile.h"
#include "alu.h"

void printALL(RegisterFile *rf, Memory *memory, int start, ALU *alu, char* tag)
{
    printRFs(rf,tag);
    printMem(memory,start,tag);
    printALUSW(alu,tag);
}
void printRFs(RegisterFile *rf,char* tag)
{
    //1. A print out of the initialized registers:
    printf("t1. A print out of the initialized registers:\n");
    printf("-------------------------------------------\n");
    
    if(tag) printf("%s\n",tag);
    int i = 0;
    for(i=0 ; i<16; i++)
    {
        printf("R%01X: %04X\n",i,rf->registers[i]);
    }
    printf("-------------------------------------------\n");
}

void printMem(Memory *memory, int start,char* tag)
{
    printf("t2. A print out of the memory:\n");
    printf("-------------------------------------\n");
    if(tag) printf("%s\n",tag);
    int i = 0;
    Byte b1;
    Byte b2;
    for(i = 0; i < 16; i = i + 2)
    {
        if(i + start < MEM_TOTAL)
        {
            mem_getb(memory,start+i,&b1);
            mem_getb(memory,start+i+1,&b2);
            printf("%04X: %02X %02X\n",i+start,b1,b2);
        }
    }
    printf("-------------------------------------------\n");
}

void printALUSW(ALU *alu, char* tag)
{
    printf("t3. A print out of the ALU and SW registers:\n");
    printf("-------------------------------------------\n");
    if(tag) printf("%s\n",tag);
    printf("ALU.A: %04X\n",alu->A);
    printf("ALU.B: %04X\n",alu->B);
    printf("ALU.R: %04X\n",alu->R);
    printf("SW   : %04X\n",*(alu->SW));
    printf("-------------------------------------------\n");
}

/*
 * 
 */
int main(int argc, char** argv) 
{
    //housekeeping
    char* err = 0;
    int i = 0;
    
    ///initialize everything
    //init memory
    Memory main_mem;
    mem_init(&main_mem);
    //init rfs
    RegisterFile rf;
    for(i=0 ; i<16; i++)
        reg_init(&(rf.registers[i]));
    //init SW
    Register SW;
    reg_init(&SW);
    //init alu
    ALU main_alu;
    alu_init(&main_alu, &SW);
    
    ////display registers
    //printALL(&rf,&main_mem,0x3000,&main_alu,"BEFORE");
    //1. initialize register file to random values
    printf("1. Initializing register file and main memory.\n");
    printf("2. Printing out initial state of memory.\n");
    
    //2. initial printout
    printALL(&rf,&main_mem,0x3000,&main_alu,"INITIAL PRINTOUT (2) ; BEFORE 3");
    
    //3. store specific values into six or more registers
    i = 2;
    rf.registers[++i]=0x0008; //R3
    printf("2. STORING %04X INTO $R%01X\n",rf.registers[i],i);
    rf.registers[++i]=0x0010; //R4
    printf("2. STORING %04X INTO $R%01X\n",rf.registers[i],i);
    rf.registers[++i]=0x1010; //R5
    printf("2. STORING %04X INTO $R%01X\n",rf.registers[i],i);
    rf.registers[++i]=0x0002; //R6
    printf("2. STORING %04X INTO $R%01X\n",rf.registers[i],i);
    rf.registers[++i]=0x0101; //R7
    printf("2. STORING %04X INTO $R%01X\n",rf.registers[i],i);
    rf.registers[++i]=0x0100; //R8
    printf("2. STORING %04X INTO $R%01X\n",rf.registers[i],i);
    printf("-------------------------------------------\n");
    printALL(&rf,&main_mem,0x3000,&main_alu,"AFTER 3 ; BEFORE 4");
    
    //4. Store the following values to memory starting at location 0x3010: 0x7F, 0xFF, 0x00, 0x04, 0xFF, 0xA2, 0x05, 0x7A.
    Byte bytes[] = {0x7F, 0xFF, 0x00, 0x04, 0xFF, 0xA2, 0x05, 0x7A};
    int start = 0x3010;
    for(i=0;i<sizeof(bytes)/sizeof(Byte);i++)
    {
        printf("4. STORING %02X IN MEM[%04X]\n",bytes[i],i+start);
        Byte temp;
        mem_setb(&main_mem, i+start, bytes[i]);
    }
    printf("-------------------------------------------\n");
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 4 ; BEFORE 5");
    
    /*5 Store the same data in $R1, $R2, $R3, and $R4 (two consecutive bytes in each, eg $R1 <- 0x7FFF)*/
    for(i=0; i<8;i+=2)
    {
        mem_getw(&main_mem,i+start,&(rf.registers[(i/2)+1]));
        printf("5. STORING %04X FROM MEM[%04X] TO $R%01X\n",(rf.registers[(i/2)+1]),i+start,(i/2)+1);
    }
    printf("-------------------------------------------\n");
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 5 ; BEFORE 6");
    
    /*6. Add $R1 and $R2*/
    printf("6. ADDING $R1 AND $R2; STORING ALU.R INTO $R0\n");
    printf("-------------------------------------------\n");
    alu_set_rs(&main_alu, rf.registers[1], rf.registers[2]);
    err = alu_op_add(&main_alu);
    alu_get_res(&main_alu, &(rf.registers[0]));
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 6 ; BEFORE 7");
    
    /*7. Multiply $R4 by $R2 (i.e. 0x057A X 0x0004 - what should the answer be in ALU.R?)*/
    printf("7. MULTIPLYING $R4 BY $R2; STORING ALU.R INTO $R0\n");
    printf("-------------------------------------------\n");
    alu_set_rs(&main_alu, rf.registers[4], rf.registers[2]);
    err = alu_op_mul(&main_alu);
    alu_get_res(&main_alu, &(rf.registers[0]));
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 7 ; BEFORE 8");

    /*8. Subtract $R4 from $R3*/
    printf("8. SUBTRACTING $R4 FROM $R3; STORING ALU.R INTO $R0\n");
    printf("-------------------------------------------\n");
    alu_set_rs(&main_alu, rf.registers[3], rf.registers[4]);
    err = alu_op_sub(&main_alu);
    alu_get_res(&main_alu, &(rf.registers[0]));
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 8 ; BEFORE 9");

    /*9. Divide $R4 by $R2 (don't forget the remainder)*/
    printf("9. DIVIDING $R4 BY $R2; STORING ALU.R INTO $R0; REMAINDER IN $R8\n");
    printf("-------------------------------------------\n");
    alu_set_rs(&main_alu, rf.registers[4], rf.registers[2]);
    err = alu_op_div(&main_alu);
    alu_get_res(&main_alu, &(rf.registers[0]));
    alu_get_res2(&main_alu, &(rf.registers[8]));
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 9 ; BEFORE 10");

    /*10. XOR $R3 and $R4*/
    printf("10. $R0 <- $R3 XOR $R4\n");
    printf("-------------------------------------------\n");
    alu_set_rs(&main_alu, rf.registers[3], rf.registers[4]);
    err = alu_op_xor(&main_alu);
    alu_get_res(&main_alu, &(rf.registers[0]));
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 10 ; BEFORE 11");

    /*11. SHL $R3 (what should the carry flag do?)*/
    printf("11. SHL $R3; STORING ALU.R INTO $R0\n");
    printf("-------------------------------------------\n");
    alu_set_rx(&main_alu, rf.registers[3]);
    err = alu_op_shl(&main_alu);
    alu_get_res(&main_alu, &(rf.registers[0]));
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 11 ; BEFORE 12");

    /*12. SHR $R2 3 times (what should the zero flag do?)*/
    printf("12. SHR $R2 3 TIMES; STORING ALU.R INTO $R2\n");
    printf("-------------------------------------------\n");
    for(i=0; i<3; i++)
    {
        alu_set_rx(&main_alu, rf.registers[2]);
        err = alu_op_shr(&main_alu);
        alu_get_res(&main_alu, &(rf.registers[2]));
    }
    printALL(&rf,&main_mem,0x300E,&main_alu,"AFTER 12");
    
    return 0;
}