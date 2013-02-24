/* 
 * File:   global.h
 * Author: David Sharer, Lawrence Grass
 *
 * Created on January 21, 2013, 1:20 PM
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include <stdint.h>

typedef uint16_t Word;
typedef unsigned char Byte;
typedef unsigned char Boolean;

typedef union {
    struct {
        unsigned char irq_lines:8;
        unsigned char reserved:4;
        unsigned char O:1;
        unsigned char C:1;
        unsigned char Z:1;
        unsigned char N:1;
    }fields;
    Word full_data;
} StatusWord;


#define FALSE 0
#define TRUE  1


#endif	/* GLOBAL_H */

