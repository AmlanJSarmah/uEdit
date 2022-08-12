#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <terminal.h>
#include <utils.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <editor.h>

// macro definations
#define CTRL_KEY(k) ((k) & 0x1f)

//original terminal configuation
struct termios original_terminal_config;

//editor configurations
struct editor_config
{
  int no_of_rows;
  int no_of_columns;
};

struct editor_config editor;

// initial editor config
void init_editor_config()
{
  if (get_window_size(&editor.no_of_rows, &editor.no_of_columns) == -1) emergency_exit("getWindowSize");
}

//gets the key that's pressed
char get_pressed_key()
{
  int no_of_byte_read;
  char c;
  while((no_of_byte_read = read(STDIN_FILENO, &c, 1)) != 1)
    if(no_of_byte_read == -1 && errno != EAGAIN) //in Cygwin when read times out rather then 0 , -1 is returned with EAGIN error so in order to make the editor compatable with Cygwin we ignroe this case.
    {
      disable_raw_mode(&original_terminal_config);
      emergency_exit("read");
    }
  return c;
}

//process key
void detect_keypress() {
  char c = get_pressed_key();
  switch (c) {
    case CTRL_KEY('q'):
      disable_raw_mode(&original_terminal_config);
      clear_screen();
      exit(0);
      break;
  }
}

int main()
{
  enable_raw_mode(&original_terminal_config); //comes from terminal.h it enables the raw mode in terminal
  init_editor_config(); //initialize editor config
  //the loop always keeps on running looking for input 
  for(;;)
  {
    write(STDOUT_FILENO,"\x1b[?25l",6); //clears cursor before repaint
    clear_screen();
    draw_rows(editor.no_of_rows,editor.no_of_columns);
    write(STDOUT_FILENO,"\x1b[?25h",6); //display the cursor
    detect_keypress();
  }
  return 0;
}