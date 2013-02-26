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
     * The file must be in hex format, 4 digits to a line. (A7A7)
     * The program starts at x3000.
     * The file is loaded immediately and the display updated.
     * New files can be loaded without restarting.
   * Step (3)
     * Runs a single integration of the fetch-decode-execute cycle.
     * All displayed information will update immediately afterward.
   * Run (2)
     * Steps through the fetch-decode-execute cycle until a HALT instruction is found, or until the program crashes due to an invalid instruction.
     * Displays are updated after each step of the fetch-decode-execute cycle.
