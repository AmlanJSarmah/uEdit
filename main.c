#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <terminal.h>
#include <utils.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string.h>

// row defination
typedef struct editor_row
{
  int size;
  char *data;
} editor_row;

// macro definations
#define CTRL_KEY(k) ((k) & 0x1f)
#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE
#endif
#ifndef _BSD_SOURCE
  #define _BSD_SOURCE
#endif
#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif  

//original terminal configuation
struct termios original_terminal_config;

//editor configurations
struct editor_config
{
  int cursor_x;
  int cursor_y;
  int no_of_rows;
  int no_of_columns;
  int no_of_text_rows;
  editor_row row;
};

struct editor_config editor;

// special keys
enum editor_special_keys {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

// initial editor config
void init_editor_config()
{
  editor.cursor_x = 0;
  editor.cursor_y = 0;
  editor.no_of_text_rows = 0;
  if (get_window_size(&editor.no_of_rows, &editor.no_of_columns) == -1) emergency_exit("getWindowSize");
}

// opens our editor
void open_editor(char *filename) 
{
  FILE *fp = fopen(filename, "r");
  if (!fp) emergency_exit("fopen");
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  linelen = getline(&line, &linecap, fp);
  if (linelen != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
      linelen--;
    editor.row.size = linelen;
    editor.row.data = malloc(linelen + 1);
    memcpy(editor.row.data, line, linelen);
    editor.row.data[linelen] = '\0';
    editor.no_of_text_rows = 1;
  }
  free(line);
  fclose(fp);
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
  // check for UP ARROW, DOWN ARROW, LEFT , RIGHT ARROW , PAGE UP and DOWN i.e. some Escape Sequence characters.
    if (c == '\x1b') 
    {
      char seq[3];
      if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
      if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
      //check for PAGE UP and PAGE DOWN keys
      if (seq[0] == '[') {
        if (seq[1] >= '0' && seq[1] <= '9') {
          if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
          if (seq[2] == '~') {
            switch (seq[1]) {
              case '1': return HOME_KEY;
              case '3': return DEL_KEY;
              case '4': return END_KEY;
              case '5': return PAGE_UP;
              case '6': return PAGE_DOWN;
              case '7': return HOME_KEY;
              case '8': return END_KEY;
            }
          }
        }
        // check for home and end key
        else if(seq[0] == 'O')
        {
          switch (seq[1]) {
            case 'H': return HOME_KEY;
            case 'F': return END_KEY;
          }
        }
        // checl for ARROW keys 
        else 
        {
          switch (seq[1]) {
            case 'A': return ARROW_UP;
            case 'B': return ARROW_DOWN;
            case 'C': return ARROW_RIGHT;
            case 'D': return ARROW_LEFT;
            case 'H': return HOME_KEY;
            case 'F': return END_KEY;
          }
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
      if(editor.cursor_x != 0)
        editor.cursor_x--;
      break;
    case ARROW_RIGHT:
      if(editor.cursor_x != editor.no_of_columns - 1)
        editor.cursor_x++;
      break;
    case ARROW_UP:
      if(editor.cursor_y != 0)
        editor.cursor_y--;
      break;
    case ARROW_DOWN:
      if(editor.cursor_y != editor.no_of_rows - 1)
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

    //page up and page down
    case PAGE_UP:
    case PAGE_DOWN:
      int no_of_iterations = editor.no_of_rows;
      while(no_of_iterations--)
        move_cursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      break;

    //home and end key
    case HOME_KEY:
      editor.cursor_x = 0;
      break;
    case END_KEY:
      editor.cursor_x = editor.no_of_columns - 1;
      break;
  }
}

int main(int argc,char *argv[])
{
  enable_raw_mode(&original_terminal_config); //comes from terminal.h it enables the raw mode in terminal
  init_editor_config(); //initialize editor config
  if(argc >= 2)
    open_editor(argv[1]);
  //the loop always keeps on running looking for input 
  for(;;)
  {
    write(STDOUT_FILENO,"\x1b[?25l",6); //clears cursor before repaint
    clear_screen();
    draw_rows(editor.no_of_rows,editor.no_of_columns,editor.no_of_text_rows,editor.row.size,editor.row.data);

    //mouse pointer buffer
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", editor.cursor_y + 1, editor.cursor_x + 1);
    write(STDOUT_FILENO,buf,strlen(buf));
    
    write(STDOUT_FILENO,"\x1b[?25h",6); //display the cursor
    detect_keypress();
  }
  return 0;
}