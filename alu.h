/* 
 * File:   alu.h
 * Author: Lemtzas
 *
 * Created on January 21, 2013, 1:17 PM
 */

#ifndef ALU_H
#define	ALU_H
#include "global.h"
#include "register.h"


typedef struct {
    Register A;
    Register B;
    Register R;
    Register R2;
    Register *SW;
} ALU;


#endif	/* ALU_H */
