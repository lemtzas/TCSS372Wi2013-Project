#include "cpu.h"

char* cpu_init(CPU *cpu) {
    cpu->halt = 0;
    return 0;
}

char* cpu_getword(CPU *cpu) {
    return mem_getw(&(cpu->memory) , cpu->MAR , &(cpu->MDR));
}

char* cpu_getbyte(CPU *cpu) {
    Byte b;
    return mem_getb(&(cpu->memory) , cpu->MAR , &b);
    cpu->MDR = b;
}

char* cpu_setword(CPU *cpu) {
    return mem_getw(&(cpu->memory) , cpu->MAR , cpu->MDR);
}

char* cpu_setbyte(CPU *cpu) {
    Byte b = cpu->MDR;
    return mem_setb(&(cpu->memory) , cpu->MAR , b);
}

char* cpu_inst_fetch(CPU *cpu) {
    cpu->MAR = cpu->PC;
    cpu->PC += 2;
    char* err = cpu_getword(cpu);
    cpu->IR = cpu->MDR;
    return err;
}

//return 0-3
char* cpu_inst_decode(CPU *cpu) {
    cpu_instruction = (cpu_instruction)(cpu->IR);
    switch(cpu_instruction.opcode.parts.op1) {
        case OP1_LDST:
            cpu_inst_LDST(cpu);
            break;
        case OP1_ALU:
            cpu_inst_ALU(cpu);
            break;
        default:
            return "not implemented";
            break;
    }
}

//return 0-3
char* cpu_inst_LDST(CPU *cpu) {
    cpu_instruction = (cpu_instruction)(cpu->IR);
    switch(cpu_instruction.opcode.parts.op2) {
        case OP2_LDI:
            return cpu_inst_LDST_LDI(cpu);
            break;
        case OP2_LDA:
            return cpu_inst_LDST_LDA(cpu);
            break;
        case OP2_LDB:
            return cpu_inst_LDST_LDB(cpu);
            break;
        case OP2_LDWMOV:
            return cpu_inst_LDST_LDWMOV(cpu);
            break;
        case OP2_STB:
            return cpu_inst_LDST_STB(cpu);
            break;
        case OP2_STW:
            return cpu_inst_LDST_STW(cpu);
            break;
        default:
            return "not implemented";
    }
}

char* cpu_inst_LDST_LDI(CPU *cpu) {
    cpu_inst_format1 ir = (cpu_inst_format1)(cpu->IR);
    cpu->rf.registers [ir->d] = ir.immed7;
    return 0;
}
char* cpu_inst_LDST_LDA(CPU *cpu) {
    cpu_inst_format0 ir = (cpu_inst_format0)(cpu->IR);
    //load and step
    cpu->MAR = cpu->PC;
    cpu->PC += 2;
    //read mem
    char* err = cpu_getword(cpu);
    //store
    cpu->rf.registers[ir.word1.d] = cpu->MDR;
    return err;
}
char* cpu_inst_LDST_LDB(CPU *cpu) {
    cpu_inst_format2 ir = (cpu_inst_format2)(cpu->IR);
    char* err;
    switch(ir.mod) {
        case MOD_MODE_REGISTER:
             // $Rd(lob) ← M[$Ra]  register mode
            cpu->MAR = cpu->rf.registers[ir.a];
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_RELATIVE:
            // $Rd(lob) ← M[BP + $Ra]  base-relative
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir.a];
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_INDEX:
            // $Rd(lob) ← M[IP-0]; IP-0 ← IP-0 + 1    index, register 0xC
            cpu->MAR = cpu->rf.registers[REG_IP_0] + cpu->rf.registers[ir.a];
            cpu->rf.registers[REG_IP_0]++;
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_INDIRECT:
            // $Rd(lob) ← M[M[$Ra]]    indirect mode
            cpu->MAR = cpu->rf.registers[ir.a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_LDWMOV(CPU *cpu) {
    cpu_inst_format2 ir = (cpu_inst_format2)(cpu->IR);
    char* err;
    switch(ir.mod) {
        case MOD_MODE_REGISTER:
            // $Rd(hob) ← M[$Ra]; $Rd(lob) ← M[$Ra + 1]
            cpu->rf.registers[ir.d] = 0;
            cpu->MAR = cpu->rf.registers[ir.a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_RELATIVE:
            // $Rd(hob) ← M[BP + $Ra];
            // $Rd(lob) ← M[BP + $Ra + 1]  base-relative
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir.a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_INDEX:
            // $Rd(hob) ← M[IP-0]; IP-0 ← IP-0 + 1;
            // $Rd(lob) ← M[IP-0]; IP-0 ← IP-0 + 1
            // index, register 0xC
            cpu->MAR = cpu->rf.registers[REG_IP_0] + cpu->rf.registers[ir.a];
            cpu->rf.registers[REG_IP_0] += 2;
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_INDIRECT:
            // $Rd(hob) ← M[M[$Ra]];
            // $Rd(lob) ← M[M[$Ra + 1]]
            // indirect mode
            cpu->MAR = cpu->rf.registers[ir.a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir.d] = cpu->MDR;
            break;
        case MOD_MODE_MOV:
            // $Rd ← ($Rs)
            // register to register move
            cpu->rf.registers[ir.d] = cpu->rf.registers[ir.a];
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_STB(CPU *cpu) {
    cpu_inst_format2 ir = (cpu_inst_format2)(cpu->IR);
    char* err;
    switch(ir.mod) {
        case MOD_MODE_REGISTER:
             // M[$Ra] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[ir.a];
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        case MOD_MODE_RELATIVE:
            // M[BP + $Ra] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir.a];
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        case MOD_MODE_INDEX:
            // M[IP-1] ← ($Rs(lob)); IP-1 ← IP-1 + 1
            // index mode, register 0xD
            cpu->MAR = cpu->rf.registers[REG_IP_1] + cpu->rf.registers[ir.a];
            cpu->rf.registers[REG_IP_1]++;
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        case MOD_MODE_INDIRECT:
            // M[M[$Ra]] ← ($Rs(lob))
            // indirect mode
            cpu->MAR = cpu->rf.registers[ir.a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_STW(CPU *cpu) {
    cpu_inst_format2 ir = (cpu_inst_format2)(cpu->IR);
    char* err;
    switch(ir.mod) {
        case MOD_MODE_REGISTER:
            // M[$Ra] ← ($Rs(hob)); M[$Ra + 1] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[ir.a];
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        case MOD_MODE_RELATIVE:
            // M[BP + $Ra] ← ($Rs(hob));
            // M[BP + $Ra + 1] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir.a];
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        case MOD_MODE_INDEX:
            // M[IP-1] ← ($Rs(hob)); IP-1 ← IP-1 + 1;
            // M[IP-1] ← ($Rs(lob)); IP-1 ← IP-1 + 1
            // index mode, register 0xD
            cpu->MAR = cpu->rf.registers[REG_IP_1] + cpu->rf.registers[ir.a];
            cpu->rf.registers[REG_IP_1]++;
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        case MOD_MODE_INDIRECT:
            // M[M[$Ra]] ← ($Rs(hob));
            // M[M[$Ra + 1]] ← ($Rs(lob))
            // indirect mode
            cpu->MAR = cpu->rf.registers[ir.a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            cpu->MDR = cpu->rf.registers[ir.d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}

char* cpu_inst_ALU(CPU *cpu) {
    cpu_instruction = (cpu_instruction)(cpu->IR);
    return cpu_instruction.opcode.parts.op2;
}

//step the "controller"
char* cpu_step(CPU *cpu) {
    char* err;
    //fetch -> decode
    err = cpu_inst_fetch(cpu); 
    err = cpu_inst_decode(cpu); if(err)return err;
    //^this function calls appropriate next function
    
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