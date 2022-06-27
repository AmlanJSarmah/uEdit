#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <terminal.h>
#include <ctype.h>

struct termios original_terminal_config;

int main()
{
  char c='\0';
  enable_raw_mode(&original_terminal_config); //comes from terminal.h it enables the raw mode in terminal
  while(1)
  {
    read(STDIN_FILENO,&c,1);
    if (iscntrl(c))
    {
      printf("%d\n\r", c);
    } 
    else 
    {
      printf("%d ('%c')\n\r", c, c);
    }
    if(c=='q') break;
  }
  disable_raw_mode(&original_terminal_config); //comes from terminal.h it disables the raw mode upon exit
  return 0;
}
