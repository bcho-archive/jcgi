#include <stdio.h>
#include <stdlib.h>

void error(const char *category)
{
    perror(category);
    abort();
}
