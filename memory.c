/*
 * Group: Dai-Gurren
 * Author: David Sharer, Lawrence Grass
 */
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
    if(((location-MEM_OFFSET) % 2) != 0) return "[mem_getw] location not even";
    if(!word) return "[mem_getw] word requires a pointer!";
    
    Byte hob = this->memory[location-MEM_OFFSET]; //get the high order byte
    Byte lob = this->memory[location-MEM_OFFSET+1]; //get the low order byte
    Word w = (hob << 8) | lob; //merge
    
    *word = w; //copy
    
    //note that the merge and copy steps could be combined
    
    return 0;
}

char* mem_setw(Memory *this , unsigned int location , Word word)
{
    //input validation
    if(!this) return "[mem_setw] *this not provided!";
    if(location-MEM_OFFSET < 0 || location-MEM_OFFSET+1 >= MEM_SIZE) return "[mem_setw] location out of bounds.";
    if(((location-MEM_OFFSET) % 2) != 0) return "[mem_setw] location not even";
    if(!word) return "[mem_setw] word requires a pointer!";
    
    Byte hob = (word>>8)&0x00FF;
    Byte lob = word&0x00FF; //TODO: I believe this is the wrong bitmask
    
    this->memory[location-MEM_OFFSET]=hob;
    this->memory[location-MEM_OFFSET+1]=lob;
    
    return 0;
}

/**Copies the Byte at the specified location into *byte. Requires Memory to be init-ed.*/
char* mem_getb(Memory *this , unsigned int location , Byte *byte)
{
    if(!this) return "[mem_getb] *this not provided!";
    if(location-MEM_OFFSET < 0 || location-MEM_OFFSET >= MEM_SIZE) return "[mem_getb] location out of bounds.";
    if(!byte) return "[mem_getb] word requires a pointer!";
    
    *byte = this->memory[location-MEM_OFFSET]; //get the byte
    
    return 0;
}

char* mem_setb(Memory *this , unsigned int location , Byte byte)
{
    if(!this) return "[mem_setb] *this not provided!";
    if(location-MEM_OFFSET < 0 || location-MEM_OFFSET >= MEM_SIZE) return "[mem_setb] location out of bounds.";
    if(!byte) return "[mem_setb] word requires a pointer!";
    
    this->memory[location-MEM_OFFSET] = byte; //get the byte
    
    return 0;
}

////Memory tests. works as of 1/27/2013
//int main(int argc, char** argv)
//{
//    //setup
//    Memory a;
//    mem_init(&a);
//    
//    //set test
//    Word b = 0x0001;
//    Word c = 0x0410;
//    Word d = 0x4001;
//    Word e;
//    Byte f = 0x04;
//    Byte g = 0x40;
//    Byte i = 0x01;
//    Byte h;
//    Byte j;
//   
//    //test setting and getting a word
//    mem_setw(&a, 0x4000, d);
//    mem_getw(&a, 0x4000, &e);
//    printf("memory at location %04x is %04x\n", 0x4000,e);
//    
//    //test for overwriting
//    mem_setw(&a, 0x4000, c);
//    mem_getw(&a, 0x4000, &e);
//    printf("and then overwritten with %04x\n",e);
//    
//    //test to see if getb and getw are the same if the values are the same
//    mem_setw(&a, 0x4000, b);
//    mem_getw(&a, 0x4000, &e);
//    mem_setb(&a, 0x4002, i);
//    mem_getb(&a, 0x4002, &h);
//    printf("byte and word equality test: byte: %04x word: %04x\n",h,e);
//  
//    //test setting and getting a byte
//    mem_setb(&a, 0x4000, f);
//    mem_getb(&a, 0x4000, &h);
//    printf("memory at location %04x is %04x\n", 0x4000,h);
//    
//    //test for overwriting
//    mem_setb(&a, 0x4000, g);
//    mem_getb(&a, 0x4000, &h);
//    printf("and then overwritten with %04x\n",h);
//    
//    //test for seeing if the bytes are in the right place
//    mem_setw(&a, 0x4000, d);
//    mem_getb(&a, 0x4000, &h);
//    mem_getb(&a, 0x4001, &j);
//    printf("Added the word %04x to memory. The bytes are: %02x and %02x",d,h,j);
// 
//    return 0;
//}
