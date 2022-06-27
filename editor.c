#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <terminal.h>
#include <utils.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

// macro definations
#define CTRL_KEY(k) ((k) & 0x1f)

//original terminal configuation
struct termios original_terminal_config;

int main()
{
  char c='\0';
  enable_raw_mode(&original_terminal_config); //comes from terminal.h it enables the raw mode in terminal
  while(1)
  {
    if(read(STDIN_FILENO,&c,1) == -1 && errno != EAGAIN) emergency_exit("read"); //in Cygwin when read times out rather then 0 , -1 is returned with EAGIN error so in order to make the editor compatable with Cygwin we ignroe this case.
    if (iscntrl(c))
    {
      printf("%d\n\r", c);
    } 
    else 
    {
      printf("%d ('%c')\n\r", c, c);
    }
    if(c==CTRL_KEY('q')) break;
  }
  disable_raw_mode(&original_terminal_config); //comes from terminal.h it disables the raw mode upon exit
  return 0;
}
