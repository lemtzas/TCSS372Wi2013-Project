#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void _cl(int y) {
    printf("\033[%d;%dH",y,0);
    fflush(stdout);
    printf("\033[K");
    fflush(stdout);
}

void _bold(char on) {
    if(on)
        printf("\033[%dm",1);
    else
        printf("\033[%dm",0);
        
}

void _err(char on) {
    if(on)
        printf("\033[%dm",31);
    else
        printf("\033[%dm",39);
        
}

void _flush_input() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
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
    y+=2; _sl(REGSPECIAL_X , y); printf("SW: %04x",cpu->SW.full_data);
    y+=2; _sl(REGSPECIAL_X , y); printf("IR: %04x",cpu->IR);
    y+=2; _sl(REGSPECIAL_X , y); printf("PC: %04x",cpu->PC);
    //printf("OSB: %04x",cpu->OSB); y+=2;
    //printf("TXB: %04x",cpu->TXT); y+=2;
    y+=6;
    y+=2; _sl(REGSPECIAL_X , y); printf("MAR: %04x",cpu->MAR);
    y+=2; _sl(REGSPECIAL_X , y); printf("MDR: %04x",cpu->MDR);
    
    return 0;
}
Register pos_prev = PC_START;
char* _debug_display_memory(CPU *cpu, Memory *memory) {
    _sl(MEMORY_X , MEMORY_Y);
    Register pos_cur = cpu->PC;
    
    //calculate start/end positions
    Register start = pos_cur - 0x20/2;
    Register end = start + 0x20;
    if(start < MEM_OFFSET ) {
        start = MEM_OFFSET;
        end = start + 0x20;
    } else if (end > MEM_TOTAL) {
        end = (Register)MEM_TOTAL;
        start = MEM_TOTAL - 0x20;
    }
    
    //print
    _bold(1);
    printf("Memory");
    _bold(0);
    int i;
    Byte b1,b2;
    char* err;
    for(i=start;i<end;i+=2) {
        _sl(MEMORY_X , MEMORY_Y+1+(i-start)/2);
        err = mem_getb(memory,i,&b1); if(err) return err;
        err = mem_getb(memory,i+1,&b2); if(err) return err;
        if(i == pos_prev || i == pos_cur)
            _bold(1);
        printf("%04X: %02X  %02X  ",i,b1,b2);
        printf("\033[%dD",2); //shift to left (spaces above are to erase)
        if(i == pos_prev) {
            _err(1);
            printf(" %c",POS_PREV_CHAR);
            _err(0);
            _bold(0);
            printf("\033[%dD",2); //shift 2 left
        }
        if(i == pos_cur) {
            _err(1);
            printf("%c",POS_CURR_CHAR);
            _err(0);
            _bold(0);
        }
    }
    
    _bold(1);
    _vert_bar(MEMORY_X+12+3,MEMORY_Y,0x11,'#');
    _vert_bar(MEMORY_X+12+4,MEMORY_Y,0x11,'#');
    _bold(0);
    
    pos_prev = pos_cur; //step previous position
    return 0;
}

char* _debug_display(CPU *cpu, Memory *memory) {
    char* err;
    err = _debug_display_regfile(cpu,memory); if(err) return err;
    err = _debug_display_regspecial(cpu,memory); if(err) return err;
    err = _debug_display_memory(cpu,memory);
    
    //UI
    _hor_bar(BOTTOM_X,BOTTOM_Y,50,'=');
    
    _sl(BOTTOM_X , BOTTOM_Y+1);
    printf("Command? ");
    _bold(0);
    printf("1) load program, 2) run program, 3) step program\n");
    
    fflush(stdout);
    return err;
}

char* debug_entry(CPU *cpu, Memory *memory) {
    if(!cpu) return "[debug_entry] cpu not defined!";
    if(!memory) return "[debug_entry] memory not defined!";
    char* err;
    err = _debug_display(cpu,memory); if(err) return err;
    _bold(1);
    
    //setup output location
    _sl(BOTTOM_X, BOTTOM_Y+10);
    printf("\033[s"); //save cursor position, for loading by CPU IN/OUT instructions
    
    int command;
    _bold(1);
    do {
        _sl(BOTTOM_X , BOTTOM_Y+2);
        printf("> ");
        if(!scanf("%i",&command)) {
            command = COMMAND_INVALID;
            fflush(stdin);
        }
    
        switch(command) {
            case COMMAND_LOAD:
                err = debug_entry_LOAD(cpu,memory); if(err) return err;
                _cl(BOTTOM_Y+2);
                break;
            case COMMAND_RUN:
                err = debug_entry_RUN(cpu,memory); if(err) return err;
                _cl(BOTTOM_Y+2);
                break;
            case COMMAND_STEP:
                err = debug_entry_STEP(cpu,memory); if(err) return err;
                _cl(BOTTOM_Y+2);
                break;
//            case COMMAND_DUMP:
//                err = debug_entry_DUMP(cpu,memory); if(err) return err;
//                _cl(BOTTOM_Y+2);
//                break;
            default:
                _sl(BOTTOM_X , BOTTOM_Y+2);
                _err(1);
                printf("                                          Invalid Option!");
                _err(0);
                break;
        }
    } while (command != COMMAND_QUIT);
    
    _bold(0);
    printf("%i",command);
    
    return 0;
}

char* debug_entry_LOAD(CPU *cpu,Memory *memory) {
    InstFile file;
    char* err;
    err = inst_file_init(&file); if(err) return err;
//    open_file(&file, "testFile.txt");
    char filename[256];
    filename[0] = 0;
    char* res = (char*)-1;
    fflush(stdin);
    do {
        _sl(BOTTOM_X+4 , BOTTOM_Y+3);
        printf("File name? ");
        if(!scanf("%s",filename)) {
            _sl(BOTTOM_X+4 , BOTTOM_Y+3); //sets the cursor location
            _err(1);
            printf("                                          Invalid Option!");
            _err(0);
            fflush(stdin);
            continue;
        }
        if(filename){
                res = open_file(&file, filename);
        }
        if((int)res == -1) {
            _err(1);
            _sl(BOTTOM_X+4 , BOTTOM_Y+3);
            printf("                            Cannot open file: %s\033[K", filename);
            _err(0);
        } else if(res > 0) {
            return res;
        }
    } while((int)res == -1);
    cpu_init(cpu);
    mem_init(memory);
    fflush(stdin);
    err = inst_copy_to_memory(&file,memory); if(err) return err;
    //_sl(BOTTOM_X+4 , BOTTOM_Y+3);
    _cl(BOTTOM_Y+3);
    err = _debug_display_memory(cpu,memory); if(err) return err;
    return 0;
}
char* debug_entry_RUN(CPU *cpu,Memory *memory) {
    char* err_buff;
    while(!(cpu->halt)) {
        err_buff = debug_entry_STEP(cpu,memory);
        if(err_buff)
            return err_buff;
    }
    return 0;
}
char* debug_entry_STEP(CPU *cpu,Memory *memory) {
    _sl(BOTTOM_X,BOTTOM_Y+10);
    init_keyboard();
    char* err =cpu_step(cpu); if(err) return err;
    return _debug_display(cpu,memory);
    close_keyboard();
}
//char* debug_entry_DUMP(CPU *cpu,Memory *memory) {
//    //is this really needed?
//}
