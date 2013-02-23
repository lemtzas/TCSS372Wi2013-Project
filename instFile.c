/*stuff*/
#include "InstFile.h"
#include <stdio.h>



char* inst_file_init(InstFile *this){
    
    this->index = 1;
    this->currentInst = 0X0000;
    printf("size: %d\n", this->index);
    int i;
    for (i = 0; i < sizeof(this->instSet)/sizeof(this->instSet[0]); i++){
        this->instSet[i] = 0x0000;
    }
    printf("size: %d\n", sizeof(this->instSet));  
    
    return 0;
}

char* open_file(InstFile *this, char *textFile){
    int instruction;
    char line[80];
    int i = 0;
    
    (this->data) = fopen (textFile, "r");  /* open the file for reading */
    if((this->data)){
        /* get a line, up to 80 chars from data.  done if NULL */
        while(fgets(line, 7, (this->data)) != NULL)//even though the line is only 4 
        {
            /* convert the string to a base 16 int */
            sscanf (line, "%04X", &instruction);
            /* add the int to the array of instructions*/
            (this->instSet[i]) = instruction;
//            printf("Instruction %d")
            i++;
        }
        fclose((this->data));  /* closes the file*/
    }
    (this->currentInst) = (this->instSet[0]);
    return 0;
}


int get_next_instruction(InstFile *this){
    return this->currentInst;
}

char* inc_next_instruction(InstFile *this){  
    if((this->index) > INST_SET_SIZE) return "[get_next_instruction] End of instruction file";
    
    if((this->index) < INST_SET_SIZE){
        this->currentInst = (this->instSet[(this->index)]);
        (this->index) += 1;
    }
    return 0;
}

char* restart_inst_file(InstFile *this){
    this->index = 1;
    (this->currentInst) = (this->instSet[0]);
    return 0;
}

//int main(){
//    InstFile file;
//    inst_file_init(&file);
//    open_file(&file, "testFile.txt");
//    
//    //test the 5 entries
//    int inst = get_next_instruction(&file);
//    printf("%04X\n", inst);
//    printf(inc_next_instruction(&file));
//    inst = get_next_instruction(&file);
//    printf("%04X\n", inst);
//    printf(inc_next_instruction(&file));
//    inst = get_next_instruction(&file);
//    printf("%04X\n", inst);
//    printf(inc_next_instruction(&file));
//    inst = get_next_instruction(&file);
//    printf("%04X\n", inst);
//    printf(inc_next_instruction(&file));
//    inst = get_next_instruction(&file);
//    printf("%04X\n", inst);
//    printf(inc_next_instruction(&file));
//    //tests that the hex printed right
//    inst = get_next_instruction(&file);
//    printf("%04X\n", inst);
//    printf(inc_next_instruction(&file));
//    //test to reset the counter and then test for out of bounds <---FAIL!!
//    restart_inst_file(&file);
//    int i;
//    for(i = 0; i < 110; i++){
//        inst = get_next_instruction(&file);
//        printf("Instruction %d: %04X\n", file.index, inst);
//        printf(inc_next_instruction(&file));
//    }
//    return 0;
//} /*of main*/
