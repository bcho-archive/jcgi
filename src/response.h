#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

#define HTTP_VERSION "HTTP1.1"

typedef enum {
    OK = 200,
    ACCEPTED = 201,
    NO_CONTENT = 204,

    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,

    INTERNAL_SERVER_ERROR = 500
} status_code_t;

typedef void (*resp_t)(char *args, char *buf, status_code_t *code);
typedef resp_t (*dispatcher_t)(char *path);

char *response_build(const struct request_header *header,
                     dispatcher_t dispatcher);
void response_destory(char *resp);
void default_resp(char *args, char *buf, status_code_t *code);

#endif
