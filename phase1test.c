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
#include "alu.h"
/*
 * 
 */
int main(int argc, char** argv) 
{
    //initialize components
    int blah;
    Memory main_mem;
    mem_init(&main_mem);
    char* err;
    
    ALU main_alu;
    Register SW;
    reg_init(&SW);
    alu_init(&main_alu, &SW);
    
    Register R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, RA, RB, RC, RD, RE, RF;
    reg_init(&R0);
    reg_init(&R1);
    reg_init(&R2);
    reg_init(&R3);
    reg_init(&R4);
    reg_init(&R5);
    reg_init(&R6);
    reg_init(&R7);
    reg_init(&R8);
    reg_init(&R9);
    reg_init(&RA);
    reg_init(&RB);
    reg_init(&RC);
    reg_init(&RD);
    reg_init(&RE);
    reg_init(&RF);
    
    printf("A print out of the initialized registers:\n");
    printf("-------------------------------------------\n");
    printf("R0: %04x\n",R0);
    printf("R1: %04x\n",R1);
    printf("R2: %04x\n",R2);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("R5: %04x\n",R5);
    printf("R6: %04x\n",R6);
    printf("R7: %04x\n",R7);
    printf("R8: %04x\n",R8);
    printf("R9: %04x\n",R9);
    printf("RA: %04x\n",RA);
    printf("RB: %04x\n",RB);
    printf("RC: %04x\n",RC);
    printf("RD: %04x\n",RD);
    printf("RE: %04x\n",RE);
    printf("RF: %04x\n",RF);
    
    //Part 3
    printf("Changing of some of the registers to computable values:\n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("R5: %04x\n",R5);
    printf("R6: %04x\n",R6);
    printf("R7: %04x\n",R7);
    printf("R8: %04x\n",R8);
    R3=0x0008;
    R4=0x0010;
    R5=0x1010;
    R6=0x0002;
    R7=0x0101;
    R8=0x0100;
    printf("AFTER:\n");
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("R5: %04x\n",R5);
    printf("R6: %04x\n",R6);
    printf("R7: %04x\n",R7);
    printf("R8: %04x\n",R8);
    printf("\n");
    
    //Part 4    
    printf("Store the following values to memory starting at\n"
           "location 0x3010: 0x7F, 0xFF, 0x00, 0x04, 0xFF,\n"
           " 0xA2, 0x05, 0x7A:\n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    Word i;
    for(i=0x3010; i<0x3018;i++)
    {
        Byte temp;
        mem_getb(&main_mem, i, &temp);
        printf("0x%04x: 0x%02x\n",i,temp);
    }
    printf("AFTER:\n");
    Byte bytes[] = {0x7F, 0xFF, 0x00, 0x04, 0xFF, 0xA2, 0x05, 0x7A};
    int j = 0;
    for(i=0x3010; i<0x3018;i++)
    {
        Byte temp;
        mem_setb(&main_mem, i, bytes[j]);
        mem_getb(&main_mem, i, &temp);
        printf("0x%04x: 0x%02x\n",i,temp);
        j++;
    }
    printf("\n");
     
    //Part5
    printf("Loading words from memory 0x3010-0x3017 into register R1-R4\n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R1: %04x\n",R1);
    printf("R2: %04x\n",R2);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("AFTER:\n");
    mem_getw(&main_mem, 0x3010, &R1);
    mem_getw(&main_mem, 0x3012, &R2);
    mem_getw(&main_mem, 0x3014, &R3);
    mem_getw(&main_mem, 0x3016, &R4);
    printf("R1: %04x\n",R1);
    printf("R2: %04x\n",R2);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("AFTER:\n");
    printf("\n");
     
    //Part 6
    printf("Add R1 and R2 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R1: %04x\n",R1);
    printf("R2: %04x\n",R2);
    printf("AFTER:\n");
    alu_set_rs(&main_alu, R1, R2);
    err = alu_op_add(&main_alu);
    alu_get_res(&main_alu, &R0);
    printf("R0: %04x\n",R0);
    printf("R1: %04x\n",R1);
    printf("R2: %04x\n",R2);
    if(err) printf("%s", err);
    printf("\n");
    
    //Part 7
    printf("Multiply R4 by R2 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R4: %04x\n",R4);
    printf("R2: %04x\n",R2);
    printf("AFTER:\n");
    alu_set_rs(&main_alu, R4, R2);
    err = alu_op_mul(&main_alu);
    alu_get_res(&main_alu, &R0);
    //*NOTE, since alu_op_mul only uses the LOB our answer is correct
    printf("R0: %04x\n",R0);
    printf("R4: %04x\n",R4);
    printf("R2: %04x\n",R2);
    if(err) printf("%s", err);
    printf("\n");
    
    //Part 8
    printf("Subtract R4 from R3 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("AFTER:\n");
    alu_set_rs(&main_alu, R3, R4);
    err = alu_op_sub(&main_alu);
    alu_get_res(&main_alu, &R0);
    printf("R0: %04x\n",R0);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    if(err) printf("%s", err);
    printf("\n");
    
    //Part 9
    printf("Divide R4 by R2 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R4: %04x\n",R4);
    printf("R2: %04x\n",R2);
    printf("R8: %04x\n",R8);
    printf("AFTER:\n");
    alu_set_rs(&main_alu, R4, R2);
    err = alu_op_div(&main_alu);
    alu_get_res(&main_alu, &R0);
    alu_get_res2(&main_alu, &R8);
    printf("R0: %04x\n",R0);
    printf("R4: %04x\n",R4);
    printf("R2: %04x\n",R2);
    printf("R8: %04x\n",R8);
    if(err) printf("%s", err);
    printf("\n");
    
    //Part 10
    printf("XOR R3 and R4 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    printf("AFTER:\n");
    alu_set_rs(&main_alu, R3, R4);
    err = alu_op_xor(&main_alu);
    alu_get_res(&main_alu, &R0);
    printf("R0: %04x\n",R0);
    printf("R3: %04x\n",R3);
    printf("R4: %04x\n",R4);
    if(err) printf("%s", err);
    printf("\n");
    
    //Part 11 <----- NOTE still need the carry info and i didn't verify the answer
    printf("SHL R3 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R3: %04x\n",R3);
    alu_set_rx(&main_alu, R3);
    err = alu_op_shl(&main_alu);
    alu_get_res(&main_alu, &R0);
    printf("AFTER:\n");
    printf("R0: %04x\n",R0);
    printf("R3: %04x\n",R3);
    if(err) printf("%s", err);
    printf("\n");
    
    //Part 12 <----- NOTE not sure how to repeat this 3 times or do the flag setting
    printf("SHR R2 \n");
    printf("-------------------------------------------\n");
    printf("BEFORE:\n");
    printf("R0: %04x\n",R0);
    printf("R2: %04x\n",R2);
    alu_set_rx(&main_alu, R2);
    err = alu_op_shr(&main_alu);
    alu_get_res(&main_alu, &R0);
    printf("AFTER:\n");
    printf("R0: %04x\n",R0);
    printf("R2: %04x\n",R2);
    if(err) printf("%s", err);
    printf("\n");
    
    return (EXIT_SUCCESS);  

}

