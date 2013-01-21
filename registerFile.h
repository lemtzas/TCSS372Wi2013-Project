/* 
 * File:   registerFile.h
 * Author: Lemtzas
 *
 * Created on January 21, 2013, 1:17 PM
 */

#ifndef REGISTERFILE_H
#define	REGISTERFILE_H
#include "global.h"

typedef struct {
    Register registers[16];
} RegisterFile;


/**Initializes RegisterFile. Requires allocated RegisterFile.*/
char* rf_init(RegisterFile *this);
/**Sets a register by number. Required init-ed RegisterFile.*/
char* rf_set(RegisterFile *this , int register_number , Word word);
/**Copies the appropriate register into word. Requires init-ed RegisterFile.*/
char* rf_get(RegisterFile *this , int register_number , Word *word);


#endif	/* REGISTERFILE_H */

