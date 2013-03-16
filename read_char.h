//modified from HKo's work here: http://www.linuxquestions.org/questions/programming-9/kbhit-34027/
// to read the most recently pressed character
// He says: "From the downloaded sources (GPL'd) of the book "Beginning Linux Programming" - Wrox."

#include <stdio.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>

void init_keyboard();
void close_keyboard();
int kbhit();
int readch();