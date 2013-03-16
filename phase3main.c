#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "register.h"
#include "registerFile.h"
#include "alu.h"
#include "cpu.h"
#include "debug.h"
#include "global.h"
#include <stdio.h>
#include "read_char.h"

int main(int argc, char** argv)
{
    CPU cpu;
    cpu_init(&cpu);
    char* err;
    err = debug_entry(&cpu,&(cpu.memory));
    if(err) {
        _bold(1);
        _err(1);
        printf("ERROR: %s", err);
    }
    return 1;

}
