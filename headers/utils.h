#include <editor.h>

void emergency_exit(const char *error);
void clear_screen();
void draw_rows(int no_of_rows,int no_of_cols);
int get_window_size(int *no_of_rows, int *no_of_columns);