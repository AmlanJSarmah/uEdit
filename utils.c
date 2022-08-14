#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

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
    //other three bytes are [2j . It clears screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // we are using 3 in write as we are writing 4 bytes in the terminal
    //The first byte is \x1b, which is the escape character, or 27 in decimal.
    //other two bytes are [H. It reposition cursor
    write(STDOUT_FILENO,"\x1b[H",3);
}

void draw_rows(int no_of_rows,int no_of_cols)
{
    int index;
    for(index = 0;index<no_of_rows;index++)
    {
        if (index == no_of_rows / 2)
        {
            char welcome_message[] = "editor --version 1";
            int welcome_message_length = strlen(welcome_message);
            if (welcome_message_length > no_of_cols) welcome_message_length = no_of_cols;
            int padding = (no_of_cols - welcome_message_length) / 2;
            if(padding)
            {
                write(STDOUT_FILENO,"~",1);
                padding--;
            }
            while(padding--) write(STDOUT_FILENO," ",1);
            write(STDOUT_FILENO, welcome_message, welcome_message_length);
        } 
        else write(STDOUT_FILENO,"~",1);
        if(index < no_of_rows - 1) write(STDOUT_FILENO,"\r\n",2);
    }
    write(STDOUT_FILENO,"\x1b[H",3);
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