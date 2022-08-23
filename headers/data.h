#ifndef _ROW__H__
#define _ROW__H__

// row defination
typedef struct editor_row
{
  int size;
  char *data;
} editor_row;

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

#endif