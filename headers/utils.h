#ifndef _UTILS__H__
#define _UTILS__H__

#include "data.h"

void emergency_exit(const char *error);
void clear_screen();
void draw_rows(struct editor_config *editor,struct write_buffer *wb);
int get_window_size(int *no_of_rows, int *no_of_columns);
void editor_scroll(struct editor_config *editor);
void write_to_buffer(struct write_buffer *wb,char* string,int size);
void write_buffer_free(struct write_buffer *wb);

#endif
