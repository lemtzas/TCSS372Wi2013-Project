#include "cpu.h"
#include "global.h"

char* cpu_init(CPU *cpu) {
    cpu->halt = FALSE;
    cpu->rf.registers[REG_SP] = SP_START;
    cpu->IR = IR_START;
    cpu->SW.full_data = SW_START;
    cpu->PC = PC_START;
    cpu->MAR = MAR_START;
    cpu->MDR = MDR_START;
    cpu->OSB = OSB_START;
    cpu->TXB = TXB_START;
    char* err = mem_init(&(cpu->memory));
    if(err) return err;
    return alu_init(&(cpu->alu), &(cpu->SW));
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
    return mem_setw(&(cpu->memory) , cpu->MAR , cpu->MDR);
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
    cpu_instruction* ir = (cpu_instruction*)(&(cpu->IR));
    switch(ir->op1) {
        case OP1_LDST:
            return cpu_inst_LDST(cpu);
            break;
        case OP1_ALU:
            return cpu_inst_ALU(cpu);
            break;
        case OP1_CONTROL:
            return cpu_inst_CONTROL(cpu);
            break;
        case OP1_SUPMISC:
            return cpu_inst_SUPMISC(cpu);
            break;
        default:
            return "not implemented";
            break;
    }
}


//various LD and ST instructions
char* cpu_inst_LDST(CPU *cpu) {
    cpu_instruction* ir = (cpu_instruction*)(&(cpu->IR));
    switch(ir->op2) {
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
        case OP2_STACK:
            return cpu_inst_LDST_STACK(cpu);
            break;
        default:
            return "opcode does not exist";
    }
    return 0;
}
char* cpu_inst_LDST_LDI(CPU *cpu) {
    cpu_inst_format1* ir = (cpu_inst_format1*)(&(cpu->IR));
    cpu->rf.registers [ir->d] = ir->immed7;
    return 0;
}
char* cpu_inst_LDST_LDA(CPU *cpu) {
    cpu_inst_format0* ir = (cpu_inst_format0*)(&(cpu->IR));
    //load and step
    cpu->MAR = cpu->PC;
    cpu->PC += 2;
    //read mem
    char* err = cpu_getword(cpu);
    //store
    cpu->rf.registers[ir->d] = cpu->MDR;
    return err;
}
char* cpu_inst_LDST_LDB(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    char* err;
    switch(ir->mod) {
        case MOD_MODE_REGISTER:
             // $Rd(lob) ← M[$Ra]  register mode
            cpu->MAR = cpu->rf.registers[ir->a];
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_RELATIVE:
            // $Rd(lob) ← M[BP + $Ra]  base-relative
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir->a];
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_INDEX:
            // $Rd(lob) ← M[IP-0]; IP-0 ← IP-0 + 1    index, register 0xC
            cpu->MAR = cpu->rf.registers[REG_IP_0] + cpu->rf.registers[ir->a];
            cpu->rf.registers[REG_IP_0]++;
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_INDIRECT:
            // $Rd(lob) ← M[M[$Ra]]    indirect mode
            cpu->MAR = cpu->rf.registers[ir->a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            err = cpu_getbyte(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_LDWMOV(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    char* err;
    switch(ir->mod) {
        case MOD_MODE_REGISTER:
            // $Rd(hob) ← M[$Ra]; $Rd(lob) ← M[$Ra + 1]
            cpu->rf.registers[ir->d] = 0;
            cpu->MAR = cpu->rf.registers[ir->a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_RELATIVE:
            // $Rd(hob) ← M[BP + $Ra];
            // $Rd(lob) ← M[BP + $Ra + 1]  base-relative
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir->a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_INDEX:
            // $Rd(hob) ← M[IP-0]; IP-0 ← IP-0 + 1;
            // $Rd(lob) ← M[IP-0]; IP-0 ← IP-0 + 1
            // index, register 0xC
            cpu->MAR = cpu->rf.registers[REG_IP_0] + cpu->rf.registers[ir->a];
            cpu->rf.registers[REG_IP_0] += 2;
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_INDIRECT:
            // $Rd(hob) ← M[M[$Ra]];
            // $Rd(lob) ← M[M[$Ra + 1]]
            // indirect mode
            cpu->MAR = cpu->rf.registers[ir->a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            err = cpu_getword(cpu); if(err) return err;
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_MODE_MOV:
            // $Rd ← ($Rs)
            // register to register move
            cpu->rf.registers[ir->d] = cpu->rf.registers[ir->a];
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_STB(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    char* err;
    switch(ir->mod) {
        case MOD_MODE_REGISTER:
             // M[$Ra] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[ir->a];
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        case MOD_MODE_RELATIVE:
            // M[BP + $Ra] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir->a];
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        case MOD_MODE_INDEX:
            // M[IP-1] ← ($Rs(lob)); IP-1 ← IP-1 + 1
            // index mode, register 0xD
            cpu->MAR = cpu->rf.registers[REG_IP_1] + cpu->rf.registers[ir->a];
            cpu->rf.registers[REG_IP_1]++;
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        case MOD_MODE_INDIRECT:
            // M[M[$Ra]] ← ($Rs(lob))
            // indirect mode
            cpu->MAR = cpu->rf.registers[ir->a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setbyte(cpu); if(err) return err;
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_STW(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    char* err;
    switch(ir->mod) {
        case MOD_MODE_REGISTER:
            // M[$Ra] ← ($Rs(hob)); M[$Ra + 1] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[ir->a];
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        case MOD_MODE_RELATIVE:
            // M[BP + $Ra] ← ($Rs(hob));
            // M[BP + $Ra + 1] ← ($Rs(lob))
            cpu->MAR = cpu->rf.registers[REG_BP] + cpu->rf.registers[ir->a];
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        case MOD_MODE_INDEX:
            // M[IP-1] ← ($Rs(hob)); IP-1 ← IP-1 + 1;
            // M[IP-1] ← ($Rs(lob)); IP-1 ← IP-1 + 1
            // index mode, register 0xD
            cpu->MAR = cpu->rf.registers[REG_IP_1] + cpu->rf.registers[ir->a];
            cpu->rf.registers[REG_IP_1]++;
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        case MOD_MODE_INDIRECT:
            // M[M[$Ra]] ← ($Rs(hob));
            // M[M[$Ra + 1]] ← ($Rs(lob))
            // indirect mode
            cpu->MAR = cpu->rf.registers[ir->a];
            err = cpu_getword(cpu); if(err) return err;
            cpu->MAR = cpu->MDR;
            cpu->MDR = cpu->rf.registers[ir->d];
            err = cpu_setword(cpu); if(err) return err;
            break;
        default:
            return "Undefined operation";
    }
    
    return 0;
}
char* cpu_inst_LDST_STACK(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    switch(ir->mod) {
        case MOD_STACK_PUSHB:
            // SP ← SP - 1; M[SP] ← ($Rs)
            //TOS points to last value pushed
            // 0123456789
            //          ^
            //         ^ 
            //         b
            cpu->rf.registers[REG_SP]--;
            cpu->MAR = cpu->rf.registers[REG_SP];
            cpu->MDR = cpu->rf.registers[ir->d];
            cpu_setbyte(cpu);
            break;
        case MOD_STACK_PUSHW:
            // SP ← SP - 1; M[SP] ← ($Rs)(lob);
            // SP ← SP - 1; M[SP] ← ($Rs)(hob)
            // 0123456789
            //          ^
            //        ^
            //        hl 
            cpu->rf.registers[REG_SP]-=2;
            cpu->MAR = cpu->rf.registers[REG_SP];
            cpu->MDR = cpu->rf.registers[ir->d];
            cpu_setword(cpu);
            break;
        case MOD_STACK_POPB:
            // $Rd(lob) ← M[SP]; SP ← SP + 1 NEED FOCUS. BAAAAAH.
            // 0123456789
            //         ^
            //         b^
            cpu->MAR = cpu->rf.registers[REG_SP];
            cpu->rf.registers[REG_SP]++;
            cpu_getbyte(cpu);
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        case MOD_STACK_POPW:
            // $Rd(hob) ← M[SP]; SP ← SP + 1;
            // $Rd(lob) ← M[SP]; SP ← SP + 1
            // 0123456789
            //        ^
            //        hl^
            cpu->MAR = cpu->rf.registers[REG_SP];
            cpu->rf.registers[REG_SP]+=2;
            cpu_getword(cpu);
            cpu->rf.registers[ir->d] = cpu->MDR;
            break;
        default:
            return "invalid value";
    }
    return 0;
}

//various ALU instructions
char* cpu_inst_ALU(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    switch(ir->op2) {
        case OP2_OPER:
            return cpu_inst_ALU_OPER(cpu);
            break;
        case OP2_SHL:
        case OP2_SHR:
            return cpu_inst_ALU_SHLR(cpu);
            break;
        default:
            return "not a valid opcode";
    }
    return 0;
}
char* cpu_inst_ALU_OPER(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    char* err = 0;
    ALU* alu = &(cpu->alu);
    //set x and y
    err = alu_set_rs(alu,cpu->rf.registers[ir->d],cpu->rf.registers[ir->a]);
    if(err) return err;
    switch(ir->mod) {
        case MOD_OPER_ADD:
            err = alu_op_add(alu); if(err) return err;
            break;
        case MOD_OPER_SUB:
            err = alu_op_sub(alu); if(err) return err;
            break;
        case MOD_OPER_MUL:
            err = alu_op_mul(alu); if(err) return err;
            break;
        case MOD_OPER_DIV:
            err = alu_op_div(alu); if(err) return err;
            err = alu_get_res2(alu,&(cpu->rf.registers[REG_RES2]));
            if(err) return err;
            break;
        case MOD_OPER_AND:
            err = alu_op_and(alu); if(err) return err;
            break;
        case MOD_OPER_OR:
            err = alu_op_or(alu); if(err) return err;
            break;
        case MOD_OPER_XOR:
            err = alu_op_xor(alu); if(err) return err;
            break;
        case MOD_OPER_NOT:
            err = alu_op_not(alu); if(err) return err;
            break;
    }
    err = alu_get_res(alu,&(cpu->rf.registers[REG_RES]));
    return err;
}
char* cpu_inst_ALU_SHLR(CPU *cpu) {
    cpu_inst_format2* ir = (cpu_inst_format2*)(&(cpu->IR));
    char* err = 0;
    ALU* alu = &(cpu->alu);
    //set x and y
    err = alu_set_rx(alu,cpu->rf.registers[ir->d]);
    switch(ir->op2) {
        case OP2_SHL:
            err = alu_op_shl(alu); if(err) return err;
            break;
        case OP2_SHR:
            err = alu_op_shr(alu); if(err) return err;
            break;
    }
    err = alu_get_res(alu,&(cpu->rf.registers[REG_RES]));
    return err;
}

//various CONTROL instructions
char* cpu_inst_CONTROL(CPU *cpu) {
    cpu_instruction* ir = (cpu_instruction*)(&(cpu->IR));
    switch(ir->op2) {
        case OP2_BR_imm11:
            return cpu_inst_CONTROL_BR_imm11(cpu);
            break;
        case OP2_BR_reg:
            return cpu_inst_CONTROL_BR_reg(cpu);
            break;
        case OP2_BRnzco_imm9:
            return cpu_inst_CONTROL_BRnzco_imm9(cpu);
            break;
        case OP2_BRnzco_reg:
            return cpu_inst_CONTROL_BRnzco_reg(cpu);
            break;
        case OP2_JSR_reg:
            return cpu_inst_CONTROL_JSR_reg(cpu);
            break;
        case OP2_JSR_nzco_reg:
            return cpu_inst_CONTROL_JSR_nzco_reg(cpu);
            break;
        case OP2_TRAP:
            return cpu_inst_CONTROL_TRAP(cpu);
            break;
        case OP2_RET:
            return cpu_inst_CONTROL_RET(cpu);
            break;
        default:
            return "invalid opcode";
    }
    return 0;
}
char* cpu_inst_CONTROL_BR_imm11(CPU *cpu){
    cpu_inst_format3* ir = (cpu_inst_format3*)(&(cpu->IR));
    cpu->PC += sext(ir->immed11,11);
    return 0;
}
char* cpu_inst_CONTROL_BR_reg(CPU *cpu){
    cpu_inst_format1* ir = (cpu_inst_format1*)(&(cpu->IR));
    cpu->PC = cpu->rf.registers[ir->d];
    return 0;
}
char* cpu_inst_CONTROL_BRnzco_imm9(CPU *cpu){
    cpu_inst_format4* ir = (cpu_inst_format4*)(&(cpu->IR));
    Boolean jump = FALSE;
    switch(ir->c) {
        case MOD_CC_N:
            if(cpu->SW.fields.N) jump = TRUE;
            break;
        case MOD_CC_Z:
            if(cpu->SW.fields.Z) jump = TRUE;
            break;
        case MOD_CC_C:
            if(cpu->SW.fields.C) jump = TRUE;
            break;
        case MOD_CC_O:
            if(cpu->SW.fields.O) jump = TRUE;
            break;
        default:
            return "invalid CC";
    }
    if(jump)
        cpu->PC += sext(ir->immed9,9);
    return 0;
}
char* cpu_inst_CONTROL_BRnzco_reg(CPU *cpu){
    cpu_inst_format5* ir = (cpu_inst_format5*)(&(cpu->IR));
    Boolean jump = FALSE;
    switch(ir->c) {
        case MOD_CC_N:
            if(cpu->SW.fields.N) jump = TRUE;
            break;
        case MOD_CC_Z:
            if(cpu->SW.fields.Z) jump = TRUE;
            break;
        case MOD_CC_C:
            if(cpu->SW.fields.C) jump = TRUE;
            break;
        case MOD_CC_O:
            if(cpu->SW.fields.O) jump = TRUE;
            break;
        default:
            return "invalid CC";
    }
    if(jump)
        cpu->PC = cpu->rf.registers[ir->r];
    return 0;
}
char* cpu_inst_CONTROL_JSR_reg(CPU *cpu){
    cpu_inst_format1* ir = (cpu_inst_format1*)(&(cpu->IR));
    cpu->PC = cpu->rf.registers[ir->d];
    
    //PUSHW(PC)
    cpu->rf.registers[REG_SP]-=2;
    cpu->MAR = cpu->rf.registers[REG_SP];
    cpu->MDR = cpu->PC;
    char* err = cpu_setword(cpu); if(err) return err;
    
    return 0;
}
char* cpu_inst_CONTROL_JSR_nzco_reg(CPU *cpu){
    cpu_inst_format5* ir = (cpu_inst_format5*)(&(cpu->IR));
    Boolean jump = FALSE;
    switch(ir->c) {
        case MOD_CC_N:
            if(cpu->SW.fields.N) jump = TRUE;
            break;
        case MOD_CC_Z:
            if(cpu->SW.fields.Z) jump = TRUE;
            break;
        case MOD_CC_C:
            if(cpu->SW.fields.C) jump = TRUE;
            break;
        case MOD_CC_O:
            if(cpu->SW.fields.O) jump = TRUE;
            break;
        default:
            return "invalid CC";
    }
    if(jump) {
        cpu->PC = cpu->rf.registers[ir->r];
    
        //PUSHW(PC)
        cpu->rf.registers[REG_SP]-=2;
        cpu->MAR = cpu->rf.registers[REG_SP];
        cpu->MDR = cpu->PC;
        char* err = cpu_setword(cpu); if(err) return err;
    }
    return 0;
}
char* cpu_inst_CONTROL_TRAP(CPU *cpu){
    return 0; //TODO: TRAP
}
char* cpu_inst_CONTROL_RET(CPU *cpu){
    cpu_inst_format3* ir = (cpu_inst_format3*)(&(cpu->IR));
    //TODO: RETI
    //POPW -> PC
    cpu->MAR = cpu->rf.registers[REG_SP];
    cpu->rf.registers[REG_SP]+=2;
    cpu_getword(cpu);
    cpu->PC = cpu->MDR;
    return 0;
}

//supervisor and misc instruction base (implements HALT and NOP)
char* cpu_inst_SUPMISC(CPU *cpu) {
    cpu_instruction* ir = (cpu_instruction*)(&(cpu->IR));
    switch(ir->op2) {
        case OP2_HALT:
            cpu->halt = 1;
            return 0;
            break;
        case OP2_NOP:
            return 0;
            break;
        case OP2_INOUT:
            return cpu_inst_SUPMISC_INOUT(cpu);
            break;
        default:
            return "not implemented or invalid operation";
    }
    return 0;
}

char* cpu_inst_SUPMISC_INOUT(CPU *cpu) {
    cpu_inst_format8* ir = (cpu_inst_format8*)(&(cpu->IR));
    switch(ir->m) {
        case MOD_INOUT_IN:
            switch(ir->p) {
                case IOPORT_KBD+3: //KBDR+3 = data1
                    //cpu->rf.registers[ir->d] = (getchar()&0xFF00)>>8;
                    cpu->rf.registers[ir->d] = cpu->rf.registers[IOPORT_KBD+3];
                    cpu->rf.registers[IOPORT_KBD] = 0x0000; //set KBD-status to no new key
                    break;
                default:
                    cpu->rf.registers[ir->d] = cpu->rf.registers[ir->p];
                    break;
            }
            break;
        case MOD_INOUT_OUT:
            switch(ir->p) {
                case IOPORT_VID+2: //VID+2 = data
                    cpu->IOPORT[IOPORT_VID] = 0x0001;
                    cpu->IOPORT[IOPORT_VID+2] = cpu->rf.registers[ir->d]&0xFF;
                    break;
                default:
                    cpu->IOPORT[ir->p] = cpu->rf.registers[ir->d]&0xFF;
                    break;
            }
            
            break;
        default:
            return "invalid mod value in IR";
    }
    return 0;
}


//step the "controller"
char* cpu_step(CPU *cpu) {
    char* err;
    
    err = check_keyboard(cpu); if(err)return err;
    err = do_output(cpu); if(err)return err;
    //fetch -> decode
    if(cpu->halt) return 0;
    err = cpu_inst_fetch(cpu); if(err)return err;
    err = cpu_inst_decode(cpu); if(err)return err;
    //^this function calls appropriate next function
    
    //decode -> LD/ST, ALU, (BR/JSR, IO/MISC)
        //LD/ST -> MOV, IMMED, ADDR, (mod)
                //(mod) -> REG, BASE, INDEX, INDIRECT
        //ALU -> EXECUTE (via alu, all have mod)
    
    return 0;
}

char* check_keyboard(CPU* cpu) {
    if(kbhit()) {
        char ch = readch();
        cpu->IOPORT[IOPORT_KBD]         = 0x0001;
        cpu->IOPORT[IOPORT_KBD+2]       = ch;
    }
    return 0;
}

char* do_output(CPU* cpu) {
    if(cpu->IOPORT[IOPORT_VID]) {
        
        
        //printf("\033[u%c\033[s",cpu->IOPORT[IOPORT_VID+2]); //load cursor position
        printf("%c",cpu->IOPORT[IOPORT_VID+2]); //load cursor position
        cpu->IOPORT[IOPORT_VID] = 0x0000;
    }
    return 0;
}

//step the "controller" until halt
//char* cpu_run(CPU *cpu) {
//    char* err_buff;
//    while(!(cpu->halt)) {
//        err_buff = cpu_step(cpu);
//        if(err_buff)
//            return err_buff;
//    }
//    return 0;
//}
//step the "controller" until halt or instruction limit

//char* cpu_run_limited(CPU *cpu, int limit) {
//    char* err_buff;
//    while(!(cpu->halt) && (limit--) > 0) {
//        err_buff = cpu_step(cpu);
//        if(err_buff)
//            return err_buff;
//    }
//    if(limit == 0)
//        return "Limit Reached";
//    return 0;
//}


uint16_t sext(Word word, int bits) {
    int16_t s = word;
    s = (s << (16-bits));
    s = s >> (16-bits);
    return s;
}

//int main() {
//    Word w = 0x01FF;
//    int bits = 8;
//    printf("sext(%04X, %d) -> %04X",(uint16_t)w,bits,(uint16_t)sext(w,bits));
//    return 0;
//}