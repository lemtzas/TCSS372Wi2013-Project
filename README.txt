# TCSS 372 Programming Project - Phase 2 #
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
 * Load a file with the option Load (1)
 * Enter a valid filename.
   * The backspace key messes this up for some reason, as do arrows. If you use them you will have to restart.
   * hex1.txt and daigurran_test.hex are provided for testing
 * Select Run (2) or Step (3), depending on how you wish the output to progress.
   * Repeat Step (3) as desired.
 * Register and Memory display will update.

## Implemented Instructions ##
 * ST/LD
   * LDI
   * LDA
   * LDB (all)
   * LDW (all)
   * MOV
   * STB (all)
   * STW (all)
   * PUSHB -needs bugfixing?
   * PUSHW -needs bugfixing?
   * POPB -needs bugfixing?
   * POPW -needs bugfixing?
 * ALU
   * ADD
   * SUB
   * MUL
   * DIV
   * AND
   * OR
   * XOR
   * NOT
   * SHL
   * SHR
 * CONTROL
   * BR (all)
   * JSR (all) -needs bugfixing
   * RET -needs bugfixing
 * SUP/MISC
   * HALT
   * NOP

## Difficulties ##
### David ###
Procrastination was my worst enemy on this one, combined with having 2 other projects to work on at the same time.

Other than that, I had some difficulty grappling with the ANSI Escape Codes to make a decent UI. I really should have started earlier and used nCurses.

### Lawrence ###
Definitely putting this off hurt.  

My biggest problem is my lack of C knowledge.  I wish i could have contributed more with the hours i spent on this project.
 
