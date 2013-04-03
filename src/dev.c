#include <stdio.h>

#include "dev.h"
#include "response.h"

static void internal_error(char *args, char *buf, status_code_t *code)
{
    sprintf(buf, "<h1>Internal Server Error</h1>");
    *code = INTERNAL_SERVER_ERROR;
}

resp_t dispatcher(char *path)
{
    return internal_error;
}
