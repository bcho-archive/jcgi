#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

struct http_header {
    char *name, *value;
    struct http_header *next;
};

struct http_header *header_create(char *name, char *value);
void header_destory(struct http_header *header);
struct http_header *header_append(struct http_header *prev,
                                  struct http_header *next);

#endif
