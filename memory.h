/* 
 * File:   memory.h
 * Author: Lemtzas
 *
 * Created on January 21, 2013, 1:17 PM
 */

#ifndef MEMORY_H
#define	MEMORY_H
#include "global.h"

//this gives 2^16 memory locations minus the 0x3000 offset
#define MEM_SIZE 0xD000
#define MEM_OFFSET 0x3000

typedef struct {
    /**offset to MEM_OFFSET*/
    Byte memory[MEM_SIZE];
} Memory;

/**Initialize Memory.*/
char* mem_init(Memory *this);
/**Copies the Byte at the specified location into *byte. Requires Memory to be init-ed.*/
char* mem_getb(Memory *this , unsigned int location , Byte *byte);
/**Copies the Word at the specified location into *word. location must be even. Requires Memory to be init-ed.*/
char* mem_getw(Memory *this , unsigned int location , Word *word);
/**Sets the byte at location to byte.*/
char* mem_setb(Memory *this , unsigned int location , Byte byte);
/**Sets the word at Location to word. location must be even.*/
char* mem_setw(Memory *this , unsigned int location , Word word);

#endif	/* MEMORY_H */

