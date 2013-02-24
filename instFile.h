/* 
 * File:   readFile.h
 * Author: Lawrence Grass
 *
 * Created on February 17, 2013, 1:37 PM
 */

#ifndef INSTFILE_H
#define	INSTFILE_H
#include "global.h"
#include <stdio.h>

#define INST_SET_SIZE 100

typedef struct {
    FILE *data;
    int instSet[INST_SET_SIZE];
    int index;
    int currentInst;
    int instruction_count;
} InstFile;

/*Initializes the ReadFile*/
char* inst_file_init(InstFile *this);

/*Opens a file and loads the instructions into instSet*/
char* open_file(InstFile *this, char *textFile);

/*Increments the currentInst to the next instruction.*/
char* inc_next_instruction(InstFile *this);

/*Gets the next instruction in the instSet*/
int get_next_instruction(InstFile *this);

/*Resets the index, so starts the instruction file over*/
char* restart_inst_file(InstFile *this);

#endif	/* INSTFILE_H */

