#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <terminal.h>

struct termios original_terminal_config;

int main()
{
  char c;
  enable_raw_mode(&original_terminal_config);
  while(read(STDIN_FILENO,&c,1) == 1 && c != 'q');
  disable_raw_mode(&original_terminal_config);
  return 0;
}
