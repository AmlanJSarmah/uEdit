#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void emergency_exit(const char *error)
{
    perror(error);
    exit(1);
}

void clear_screen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
}