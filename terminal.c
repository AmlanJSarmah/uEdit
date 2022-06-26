#include <stdio.h>
#include <terminal.h>
#include <termios.h>
#include <unistd.h>

void enable_raw_mode(struct termios *original_terminal_config)
{ 
    tcgetattr(STDIN_FILENO, original_terminal_config);
    struct termios raw_config = *original_terminal_config;
    raw_config.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_config);
}

void disable_raw_mode(struct termios *original_terminal_config)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original_terminal_config);
}