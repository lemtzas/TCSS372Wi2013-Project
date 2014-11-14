/*
 * File: register.c
 * Group: Dai-Gurran
 */
#include "register.h"

char* reg_init(Register *this)
{
    if(!this) "[reg_init] register not provided!";
    (*this) = 0;
    return 0;
}
/**Set Low Order Byte.*/
char* reg_setLOB(Register* this, Byte b)
{
    if(!this) "[reg_setLOB] register not provided!";
    (*this) &= 0xFF00;    //mask out lower byte
    (*this) |= b;       //copy lower byte
    return 0;
}
/**Set High Order Byte.*/
char* reg_setHOB(Register* this, Byte b)
{
    if(!this) "[reg_setHOB] register not provided!";
    (*this) &= 0x00FF;    //mask out upper byte
    (*this) |= b << 8;  //copy upper byte
    return 0;
}
/**Set Low Order Byte.*/
char* reg_getLOB(Register* this, Byte* b)
{
    if(!this)   "[reg_getLOB] register not provided!";
    if(!b)      "[reg_getLOB] byte location not provided!";
    (*b) = (*this) & 0x00FF; //mask and set (masking could be ignored)
    return 0;
}
/**Set High Order Byte.*/
char* reg_getHOB(Register* this, Byte* b)
{
    if(!this)   "[reg_getHOB] register not provided!";
    if(!b)      "[reg_getHOB] byte location not provided!";
    (*b) = ((*this) & 0xFF00) >> 8; //mask, shift, and set(masking could be ignored)
    return 0;
}

//Test successful as of 2013.01.27 02:46
//int main(int argc, char** argv)
//{
//    //setup
//    Register a,b,c,d;
//    reg_init(&a); reg_init(&b); reg_init(&c); reg_init(&d);
//    
//    //set test
//    a = 0x0041;
//    b = 0x4100;
//    c = 0x0410;
//    d = 0x4001;
//    printf("set     \t%04x\t%04x\t%04x\t%04x |\t\n",a,b,c,d);
//    
//    //setLOB test
//    reg_setLOB(&a,0x80);
//    reg_setLOB(&b,0x80);
//    reg_setLOB(&c,0x80);
//    reg_setLOB(&d,0x80);
//    printf("setLOB 80\t%04x\t%04x\t%04x\t%04x |\t\n",a,b,c,d);
//    
//    //getHOB test
//    Byte ba,bb,bc,bd;
//    reg_getHOB(&a,&ba);
//    reg_getHOB(&b,&bb);
//    reg_getHOB(&c,&bc);
//    reg_getHOB(&d,&bd);
//    printf("getHOB  \t%02x\t%02x\t%02x\t%02x   |\t\n",ba,bb,bc,bd);
//    
//    //setHOB test
//    reg_setHOB(&a,0x80);
//    reg_setHOB(&b,0x80);
//    reg_setHOB(&c,0x80);
//    reg_setHOB(&d,0x80);
//    printf("setHOB 80\t%04x\t%04x\t%04x\t%04x |\t\n",a,b,c,d);
//    
//    //getLOB test
//    reg_getLOB(&a,&ba);
//    reg_getLOB(&b,&bb);
//    reg_getLOB(&c,&bc);
//    reg_getLOB(&d,&bd);
//    printf("getLOB  \t  %02x\t  %02x\t  %02x\t  %02x |\t\n",ba,bb,bc,bd);
//    
//    return 0;
//}
