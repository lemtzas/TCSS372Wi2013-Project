#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "memory.h"

#define MEMORY_X        1
#define MEMORY_Y        1
#define REGFILE_X       MEMORY_X+12+8
#define REGFILE_Y       1
#define REGSPECIAL_X    REGFILE_X+14
#define REGSPECIAL_Y    1
#define BOTTOM_X        1
#define BOTTOM_Y        18

#define DEBUG_MEMORY_START 0x3000

//set the cursor location
void _sl(int x, int y) {
    printf("\033[%d;%dH",y,x);
}

void _bold(char on) {
    if(on)
        printf("\033[%dm",1);
    else
        printf("\033[%dm",0);
        
}

void _vert_bar(int x, int y, int h, char c)
{
    int i;
    for(i=0;i<h;i++) {
        _sl(x,y+i);
        printf("%c",c);
    }
}

void _hor_bar(int x, int y, int w, char c)
{
    int i;
    _sl(x,y);
    for(i=0;i<w;i++) {
        printf("%c",c);
    }
}

char* _debug_display_regfile(CPU *cpu, Memory *memory) {
    _sl(REGFILE_X , REGFILE_Y);
    _bold(1);
    printf("Registers");
    _bold(0);
    int i;
    for(i=0;i<=0xF;i++) {
        _sl(REGFILE_X , REGFILE_Y+1+i);
        printf("$R%01X: %04X", i, cpu->rf.registers[i]);
    }
    
    _bold(1);
    _vert_bar(REGFILE_X+11,REGFILE_Y,0x11,'|');
    _bold(0);
    
    return 0;
}


char* _debug_display_regspecial(CPU *cpu, Memory *memory) {
    _sl(REGSPECIAL_X , REGSPECIAL_Y);
    _bold(1);
    printf("Special");
    _bold(0);
    int y = REGSPECIAL_Y;
    y+=2; _sl(REGSPECIAL_X , y); printf("SW: %04x",cpu->SW);
    y+=2; _sl(REGSPECIAL_X , y); printf("IR: %04x",cpu->IR);
    y+=2; _sl(REGSPECIAL_X , y); printf("PC: %04x",cpu->PC);
    //printf("OSB: %04x",cpu->OSB); y+=2;
    //printf("TXB: %04x",cpu->TXT); y+=2;
    y+=6;
    y+=2; _sl(REGSPECIAL_X , y); printf("MAR: %04x",cpu->MAR);
    y+=2; _sl(REGSPECIAL_X , y); printf("MDR: %04x",cpu->MDR);
    
    return 0;
}

char* _debug_display_memory(CPU *cpu, Memory *memory) {
    _sl(MEMORY_X , MEMORY_Y);
    _bold(1);
    printf("Memory");
    _bold(0);
    int i;
    Byte b1,b2;
    for(i=0;i<0x10;i++) {
        _sl(MEMORY_X , MEMORY_Y+1+i);
        mem_getb(memory,DEBUG_MEMORY_START+(2*i),&b1);
        mem_getb(memory,DEBUG_MEMORY_START+(2*i)+1,&b2);
        printf("%04X: %02X  %02X",DEBUG_MEMORY_START+(2*i),b1,b2);
    }
    
    _bold(1);
    _vert_bar(MEMORY_X+12+3,MEMORY_Y,0x11,'#');
    _vert_bar(MEMORY_X+12+4,MEMORY_Y,0x11,'#');
    _bold(0);
    return 0;
}

char* _debug_display(CPU *cpu, Memory *memory) {
    _debug_display_regfile(cpu,memory);
    _debug_display_regspecial(cpu,memory);
    _debug_display_memory(cpu,memory);
    return 0;
}

char* debug_entry(CPU *cpu, Memory *memory) {
    if(!cpu) return "[debug_entry] cpu not defined!";
    if(!memory) return "[debug_entry] memory not defined!";
    _debug_display(cpu,memory);
    _bold(1);
    _hor_bar(BOTTOM_X,BOTTOM_Y,50,'=');
    _sl(BOTTOM_X , BOTTOM_Y+1);
    printf("Command? ");
    _bold(0);
    printf("1) load program, 2) run program, 3) step program, 4) dump memory");
    
    _bold(1);
    printf("\n> ");
    int command;
    scanf("%i",&command);
    _bold(0);
    
    printf("%i",command);
    
    return 0;
}