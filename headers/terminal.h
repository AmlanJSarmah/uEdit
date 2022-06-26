#include <termios.h>
void enable_raw_mode(struct termios *original_terminal_config);
void disable_raw_mode(struct termios *original_terminal_config);