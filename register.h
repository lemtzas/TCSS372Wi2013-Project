/* 
 * File:   register.h
 * Author: Lemtzas
 *
 * Created on January 21, 2013, 1:17 PM
 */

#ifndef REGISTER_H
#define	REGISTER_H
#include "global.h"

typedef unsigned short Register;

/**Initialize to 0.*/
char* reg_init(Register *this);
/**Set Low Order Byte.*/
char* reg_setLOB(Register* this, Byte b);
/**Set High Order Byte.*/
char* reg_setHOB(Register* this, Byte b);
/**Set Low Order Byte.*/
char* reg_getLOB(Register* this, Byte* b);
/**Set High Order Byte.*/
char* reg_getHOB(Register* this, Byte* b);


#endif	/* REGISTER_H */

