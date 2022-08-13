#ifndef _TERMINAL__H__
#define _TERMINAL__H__

void enable_raw_mode(struct termios *original_terminal_config);
void disable_raw_mode(struct termios *original_terminal_config);

#endif