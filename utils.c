#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

void emergency_exit(const char *error)
{
    perror(error);
    exit(1);
}