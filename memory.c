#include "memory.h"
#include "global.h"
char* mem_init(Memory* this)
{
    int i;
    for(i = 0; i < MEM_SIZE; i++)
        this->memory[i] = 0x0000;
    return 0;
}

char* mem_getw(Memory *this , unsigned int location , Word *word)
{
    //input validation
    if(!this) return "[mem_getw] *this not provided!";
    if(location-MEM_OFFSET < 0 || location-MEM_OFFSET+1 >= MEM_SIZE) return "[mem_getw] location out of bounds.";
    if(location-MEM_OFFSET % 2 != 0) return "[mem_getw] location not even";
    if(!word) return "[mem_getw] word requires a pointer!";
    
    Byte hob = this->memory[location-MEM_OFFSET]; //get the high order byte
    Byte lob = this->memory[location-MEM_OFFSET+1]; //get the low order byte
    Word w = (hob << 8) & lob; //merge
    
    *word = w; //copy
    
    //note that the merge and copy steps could be combined
    
    return 0;
}