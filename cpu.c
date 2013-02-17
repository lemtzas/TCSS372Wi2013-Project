#include "cpu.h"

char* cpu_init(CPU *cpu) {
    cpu->halt = 0;
    return 0;
}

//step the "controller"
char* cpu_step(CPU *cpu) {
    //TODO: STATE TRANSITION; SHOULD WE DO THIS WITH MICROCODE OR?
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