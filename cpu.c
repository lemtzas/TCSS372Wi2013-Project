#include "cpu.h"

char* cpu_init(CPU *cpu) {
    cpu->halt = 0;
    return 0;
}

void cpu_inst_fetch(CPU *cpu) {
    
}

//return 
int cpu_inst_decode(CPU *cpu) {
    
}

int cpu_inst_LDST(CPU *cpu) {
    
}

int cpu_inst_ALU(CPU *cpu) {
    
}

//step the "controller"
char* cpu_step(CPU *cpu) {
    //fetch -> decode
    cpu_inst_fetch(cpu);
    switch(cpu_inst_decode(cpu)) {
       
    }
    //decode -> LD/ST, ALU, (BR/JSR, IO/MISC)
        //LD/ST -> MOV, IMMED, ADDR, (mod)
                //(mod) -> REG, BASE, INDEX, INDIRECT
        //ALU -> EXECUTE (via alu, all have mod)
    
    return 0;
}

//step the "controller" until halt
char* cpu_run(CPU *cpu) {
    char* err_buff;
    while(!(cpu->halt)) {
        err_buff = cpu_step(cpu);
        if(err_buff)
            return err_buff;
    }
    return 0;
}

//step the "controller" until halt or instruction limit
char* cpu_run_limited(CPU *cpu, int limit) {
    char* err_buff;
    while(!(cpu->halt) && (limit--) > 0) {
        err_buff = cpu_step(cpu);
        if(err_buff)
            return err_buff;
    }
    if(limit == 0)
        return "Limit Reached";
    return 0;
}