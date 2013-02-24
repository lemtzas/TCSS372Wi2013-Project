/* 
 * File:   debug.h
 * Author: David
 *
 * Created on February 16, 2013, 1:17 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H
#include "cpu.h"
#include "registerFile.h"
#include "register.h"
#include "memory.h"

#define COMMAND_INVALID -1
#define COMMAND_QUIT    0
#define COMMAND_LOAD    1
#define COMMAND_RUN     2
#define COMMAND_STEP    3
#define COMMAND_DUMP    4

#include "instFile.h"

char* debug_entry(CPU *cpu,Memory *memory);
char* debug_entry_LOAD(CPU *cpu,Memory *memory);
char* debug_entry_RUN(CPU *cpu,Memory *memory);
char* debug_entry_STEP(CPU *cpu,Memory *memory);
char* debug_entry_DUMP(CPU *cpu,Memory *memory);


#endif	/* DEBUG_H */

