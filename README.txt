# TCSS 372 Programming Project - Phase 3 #
## Coders ##
 * Team Daigurren
   * David Sharer
   * Lawrence Grass

## What to Expect ##
 * Three columns with relevant information.
   * Memory
     * Displays the relevant sections of memory.
     * The previous PC location is marked by a red "-"
     * The current PC location is marked by a "<"
   * Registers
     * Displays the contents of all relevant registers in hex
   * Special
     * Displays the contents of all relevant "special" registers in hex.
 * Three Command Options, selected by entering the appropriate number
   * Load (1)
     * Allows you to enter the path of a file to load.
     * The file must be in hex format, 4 digits to a line, consecutively placed (A7A7)
     * The program starts at x3000.
     * The file is loaded immediately and the display updated.
     * New files can be loaded without restarting.
   * Step (3)
     * Runs a single integration of the fetch-decode-execute cycle.
     * All displayed information will update immediately afterward.
   * Run (2)
     * Steps through the fetch-decode-execute cycle until a HALT instruction is found, or until the program crashes due to an invalid instruction.
     * Displays are updated after each step of the fetch-decode-execute cycle.

## Operation ##
 * Load with the option Load (1)
 * Enter InOutTest.txt as the filename.
   * The backspace key messes this up for some reason, as do arrows. If you use them you will have to restart.
 * Select Run (2)
 * Register and Memory display will update.

## Implemented Instructions ##
 * I/O
    * IN
    * OUT

## Difficulties ##
### David ###
Procrastination was my worst enemy on this one, combined with having 2 other projects to work on at the same time.

Other than that, I had some difficulty grappling with the ANSI Escape Codes to make a decent UI. I really should have started earlier and used nCurses.

### Lawrence ###
A large amount of confusion on what exactly we were supposed to implement made it hard to get started. Once he sent the e-mail refining the tasks it got easier.

Also sickness made this one difficult to, David was sick the weekend before it was due and I was sick the day it was due.

 
