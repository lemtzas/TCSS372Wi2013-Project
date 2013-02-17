#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "register.h"
#include "registerFile.h"
#include "alu.h"
#include "debug.h"

//set the cursor location
void _sl(int x, int y)
{
    printf("\033[%d;%dH",x,y);
}

//set the cursor color
//go to http://en.wikipedia.org/wiki/ANSI_escape_code#graphics
// search SGR
//void _sc(int first, ...)
//{
//    printf("\033[%d;%dm",x,y);
//}

int main(int argc, char** argv)
{
    printf("first line");
    _sl(20,30);
    printf("batman");
}
