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
    raw_config.c_lflag &= ~(ISIG); //sends a SIFINT signal to disable the Ctrl + C behaviour
    raw_config.c_lflag &= ~(IEXTEN); // Disable the behaviour of Ctrl + V
    raw_config.c_iflag &= ~(IXON); // pauses Ctrl + S and Ctrl + Q pause and resume behaviour 
    raw_config.c_oflag &= ~(OPOST); // when printing a new line we get \n\r here \r is responsible to move the cursor to end of line we don't want this as we want our code to be indented
    raw_config.c_cc[VMIN] = 0; //specifies the minimum amount of chracter require before input
    raw_config.c_cc[VTIME] = 1; //specifies maximum amount of time before reading here we set it to 100 milisecond
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_config);
}

void disable_raw_mode(struct termios *original_terminal_config)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original_terminal_config);
}

// TCSAFLUSH : The TCSAFLUSH argument specifies when to apply the change