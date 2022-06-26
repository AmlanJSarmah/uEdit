#include <stdio.h>
#include <terminal.h>
#include <termios.h>
#include <unistd.h>


// Editing a BITFLAG with BITWISE NOT(~) and AND(&). Let us consider a bitflag:
// X = 00000000000000000000000000001000(this is the ECHO bitflag)
// now , ~(X) = X' = 11111111111111111111111111110111
// now , X & X' =  00000000000000000000000000000000
// we successfully edited the value of the 4th bitflag here in this example it helped us turn off the ECHO feature


void enable_raw_mode(struct termios *original_terminal_config)
{ 
    tcgetattr(STDIN_FILENO, original_terminal_config);
    struct termios raw_config = *original_terminal_config;
    raw_config.c_lflag &= ~(ECHO); //turns off ECHO by editing the bitflag ECHO causes each keypress to be displayed.
    raw_config.c_lflag &= ~(ICANON); //turns off ICANON . Now we read data byte by byte rather then pressing ENTER for every line
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_config);
}

void disable_raw_mode(struct termios *original_terminal_config)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original_terminal_config);
}

// TCSAFLUSH : The TCSAFLUSH argument specifies when to apply the change