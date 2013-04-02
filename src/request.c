#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request.h"

static int create_str(char **dest, const char *src)
{
    *dest = malloc(sizeof(char) * strlen(src));
    if (*dest != NULL) {
        strcpy(*dest, src);
        return 0;
    }
    return -1;
}

struct request_header *request_parse(char *request)
{
    struct request_header *header;
    char method[strlen(request)];
    char path[strlen(request)];
    char protocol[strlen(request)];

    header = malloc(sizeof(struct request_header));
    if (header != NULL) {
        sscanf(request, "%s %s %s", method, path, protocol);
        create_str(&header->method, method);
        create_str(&header->path, path);
        create_str(&header->protocol, protocol);
    }

    return header;
}

int request_validate(struct request_header *header)
{
    if (header == NULL)
        return 0;
    if (strcmp(header->protocol, "HTTP/1.0") &&
        strcmp(header->protocol, "HTTP/1.1"))
        return 0;
    return 1;
}

void request_dispatch(struct request_header *header, char *resp)
{
    sprintf(resp, ""
   "HTTP/1.1 200 OK\n"
   "Content-Type: text/json \n"
   "\n"
   "{hello: \"world\"}");
}

void request_destory(struct request_header *header)
{
    if (header != NULL) {
        free(header->method);
        free(header->path);
        free(header->protocol);
        free(header);
    }
}
