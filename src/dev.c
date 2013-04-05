#include <stdio.h>

#include "dev.h"
#include "response.h"

static void internal_error(char *args, char *buf, status_code_t *code,
                           char *content_type)
{
    sprintf(buf, "<h1>Internal Server Error</h1>");
    *code = INTERNAL_SERVER_ERROR;
    sprintf(content_type, "text/html");
}

resp_t dispatcher(char *path)
{
    return internal_error;
}
