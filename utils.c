#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "data.h"
#include "utils.h"

// Note on Escape sequence character:
// It always begin with \x1b which is equivalent to 27. It instruct the terminal to do various text formatting tasks, such as coloring text, moving the cursor around, and clearing parts of the screen.
//It is always followed by a [

// Understanding "\x1b[2J"
// J is erase in display command to erase parts of screen it is precceded by a 2 which is the argument for J.
// 2 means clearing entire screen while 1 clear screen up to the cursor from the start and 0 will clear screen from cursor to the end. 0 is the default argument.


void emergency_exit(const char *error)
{
    clear_screen();
    perror(error);
    exit(1);
}

void clear_screen()
{
    // we are using 4 in write as we are writing 4 bytes in the terminal
    //The first byte is \x1b, which is the escape character, or 27 in decimal.
    //write(STDIN_FILENO,"\x1b[2J",5);
    // We wil be using a alternative way 
    write(STDIN_FILENO,"\033c",3);
    // we are using 3 in write as we are writing 3 bytes in the terminal
    //The first byte is \x1b, which is the escape character, or 27 in decimal.
    //other two bytes are [H. It reposition cursor
    write(STDOUT_FILENO,"\x1b[H",3);
}

void draw_rows(struct editor_config *editor,struct write_buffer *wb)
{
    int index;
    struct editor_config editor_new = *editor;
    for(index = 0;index < editor_new.no_of_rows;index++)
    {
      int file_row = index + editor_new.row_offset;
        if(file_row >= editor_new.no_of_text_rows)
        {
            if (editor_new.no_of_text_rows == 0 && index == editor_new.no_of_rows / 2)
            {
                char welcome_message[] = "Editor --Version 1";
                int welcome_message_length = strlen(welcome_message);
                if (welcome_message_length > editor_new.no_of_columns) welcome_message_length = editor_new.no_of_columns;
                int padding = (editor_new.no_of_columns - welcome_message_length) / 2;
                if(padding)
                {
                    write_to_buffer(wb,"\r~",2);
                    padding--;
                }
                while(padding--) write_to_buffer(wb," ",1);
                write_to_buffer(wb, welcome_message, welcome_message_length);
            } 
            else write_to_buffer(wb,"\r~",2);
            if(index < editor_new.no_of_rows - 1) write_to_buffer(wb,"\r\n",2);
        }
        else
        {
            int len = editor_new.row[file_row].size;
            if (len > editor_new.no_of_columns) len = editor_new.no_of_columns;
            write_to_buffer(wb, editor_new.row[file_row].data, len);
            write_to_buffer(wb,"\n\r",2);
        }
    }
    write_to_buffer(wb,"\x1b[H",3); //reposition the cursor to the top
    *editor = editor_new;
}

int get_window_size(int *no_of_rows, int *no_of_columns)
{
    struct winsize window_size;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == -1 || window_size.ws_col == 0) return -1;
    else
    {
        *no_of_columns = window_size.ws_col;
        *no_of_rows = window_size.ws_row;
        return 0;
    } 
}

void editor_scroll(struct editor_config *editor)
{
  if(editor->cursor_y < editor->row_offset) editor->row_offset = editor->cursor_y;
  if (editor->cursor_y >= editor->row_offset + editor->no_of_rows) editor->row_offset = editor->cursor_y - editor->no_of_rows + 1;
}

void write_to_buffer(struct write_buffer *wb,char* string,int size)
{
  char *updated_buffer = realloc(wb->string, wb->size + size);
  if (updated_buffer == NULL) return;
  memcpy(&updated_buffer[wb->size], string, size);
  wb->string = updated_buffer;
  wb->size += size;
}

void write_buffer_free(struct write_buffer *wb) {
  free(wb->string);
}
