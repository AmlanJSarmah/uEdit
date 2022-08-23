#ifndef _UTILS__H__
#define _UTILS__H__

#include <data.h>

void emergency_exit(const char *error);
void clear_screen();
void draw_rows(struct editor_config *editor);
int get_window_size(int *no_of_rows, int *no_of_columns);

#endif