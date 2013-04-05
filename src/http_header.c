#include <stdlib.h>
#include <string.h>

#include "http_header.h"
#include "error.h"

struct http_header *header_create(char *name, char *value)
{
    struct http_header *header;

    header = malloc(sizeof(struct http_header));
    if (header == NULL)
        error("Create header");
    header->name = malloc(sizeof(char) * strlen(name));
    header->value = malloc(sizeof(char) * strlen(value));
    if (header->name == NULL || header->value == NULL)
        error("Create header");
    strcpy(header->name, name);
    strcpy(header->value, value);
    header->next = NULL;

    return header;
}

static void header_free(struct http_header *header)
{
    if (header != NULL) {
        free(header->name);
        free(header->value);
        free(header);
    }
}

void header_destory(struct http_header *header)
{
    struct http_header *next;
    for (next = header;header != NULL;) {
        next = header->next;
        header_free(header);
        header = next;
    }
}

struct http_header *header_append(struct http_header *prev,
                                  struct http_header *next)
{
    if (prev == NULL)
        return next;
    next->next = prev->next;
    prev->next = next;

    return prev;
}
