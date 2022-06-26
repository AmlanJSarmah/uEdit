#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <terminal.h>

struct termios original_terminal_config;

int main()
{
  char c;
  enable_raw_mode(&original_terminal_config); //comes from terminal.h it enables the raw mode in terminal
  while(read(STDIN_FILENO,&c,1) == 1 && c != 'q'); // we use read() so that we don't generate any input buffer.
  disable_raw_mode(&original_terminal_config); //comes from terminal.h it disables the raw mode upon exit
  return 0;
}
