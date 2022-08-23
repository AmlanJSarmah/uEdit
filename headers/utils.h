#ifndef _UTILS__H__
#define _UTILS__H__

void emergency_exit(const char *error);
void clear_screen();
void draw_rows(int no_of_rows, int no_of_cols,int no_of_text_rows,int text_size,char *data);
int get_window_size(int *no_of_rows, int *no_of_columns);

#endif