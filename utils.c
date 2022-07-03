#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Note on Escape sequence character:
// It always begin with \x1b which is equivalent to 27. It instruct the terminal to do various text formatting tasks, such as coloring text, moving the cursor around, and clearing parts of the screen.
// J is erase in display command to erase parts of screen it is precceded by a 2 which is the argument for J.
// 2 means clearing entire screen while 1 clear screen up to the cursor from the start and 0 will clear screen from cursor to the end. 0 is the default argument.


void emergency_exit(const char *error)
{
    perror(error);
    exit(1);
}

void clear_screen()
{
    // we are using 4 in write as we are writing 4 bytes in the terminal
    //The first byte is \x1b, which is the escape character, or 27 in decimal.
    //other three bytes are [2j
    write(STDOUT_FILENO, "\x1b[2J", 4);
}