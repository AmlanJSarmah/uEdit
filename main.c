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

// macro definations
#define CTRL_KEY(k) ((k) & 0x1f)

//original terminal configuation
struct termios original_terminal_config;

//editor configurations
struct editor_config
{
  int cursor_x;
  int cursor_y;
  int no_of_rows;
  int no_of_columns;
};

struct editor_config editor;

// special keys
enum editor_special_keys {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN
};

// initial editor config
void init_editor_config()
{
  editor.cursor_x = 0;
  editor.cursor_y = 0;
  if (get_window_size(&editor.no_of_rows, &editor.no_of_columns) == -1) emergency_exit("getWindowSize");
}

//gets the key that's pressed
int get_pressed_key()
{
  int no_of_byte_read;
  char c;
  while((no_of_byte_read = read(STDIN_FILENO, &c, 1)) != 1)
    if(no_of_byte_read == -1 && errno != EAGAIN) //in Cygwin when read times out rather then 0 , -1 is returned with EAGIN error so in order to make the editor compatable with Cygwin we ignroe this case.
    {
      disable_raw_mode(&original_terminal_config);
      emergency_exit("read");
    }
  // check for UP ARROW, DOWN ARROW, LEFT and RIGHT ARROW
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
    if (seq[0] == '[') {
      switch (seq[1]) {
        case 'A': return ARROW_UP;
        case 'B': return ARROW_DOWN;
        case 'C': return ARROW_RIGHT;
        case 'D': return ARROW_LEFT;
      }
    }
    return '\x1b';
  } 
  else return c;
}

//move cursor
void move_cursor(int pressed_key)
{
  switch(pressed_key)
  {
     case ARROW_LEFT:
      editor.cursor_x--;
      break;
    case ARROW_RIGHT:
      editor.cursor_x++;
      break;
    case ARROW_UP:
      editor.cursor_y--;
      break;
    case ARROW_DOWN:
      editor.cursor_y++;
      break;
  }
}

//process key
void detect_keypress() {
  int c = get_pressed_key();
  switch (c) {
    //qutting editor
    case CTRL_KEY('q'):
      disable_raw_mode(&original_terminal_config);
      clear_screen();
      exit(0);
      break;

    //cursor movement
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      move_cursor(c);
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

    //mouse pointer buffer
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", editor.cursor_y + 1, editor.cursor_x + 1);
    write(STDOUT_FILENO,buf,strlen(buf));
    
    write(STDOUT_FILENO,"\x1b[?25h",6); //display the cursor
    detect_keypress();
  }
  return 0;
}