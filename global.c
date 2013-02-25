#include "global.h"
#include <stdarg.h>
#include <stdlib.h>

char* errstr( char* format, ...) {
    char buffer[512];
    va_list args;
    va_start (args, format);
    int count = vsprintf(buffer, format, args);
    va_end(args);
    char* res = malloc(sizeof(char) * (count+1));
    int i;
    for(i = 0; i <= count; i++) {
        res[i] = buffer[i];
    }
    return res;
}