#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "response.h"
#include "error.h"

#define MAX_BODY_LEN 1024 * 100
#define MAX_HEADER_LEN 1024
#define MAX_STATUS_LINE_LEN 40

#define CRLF "\r\n"

static char *reason_parse(status_code_t code)
{
    if (code == OK)
        return "OK";
    if (code == ACCEPTED)
        return "Accepted";
    if (code == NO_CONTENT)
        return "No Content";

    if (code == BAD_REQUEST)
        return "Bad Request";
    if (code == FORBIDDEN)
        return "Forbidden";
    if (code == NOT_FOUND)
        return "Not Found";
    if (code == METHOD_NOT_ALLOWED)
        return "Method Not Allowed";
    
    if (code == INTERNAL_SERVER_ERROR)
        return "Internal Server Error";

    return "Internal Server Error";
}

static void response_status_line(status_code_t status_code, char *status_line)
{
    sprintf(status_line, "%s %d %s\r\n", HTTP_VERSION, status_code,\
            reason_parse(status_code));
}

static void response_header(char *header)
{
    sprintf(header, "\r\n");
}

char *response_build(const struct request_header *header,
                     dispatcher_t dispatcher)
{
    resp_t func;
    char *resp, *resp_header, *body;
    status_code_t code;

    resp= malloc(sizeof(char) * MAX_STATUS_LINE_LEN);
    resp_header = malloc(sizeof(char) * MAX_HEADER_LEN);
    body = malloc(sizeof(char) * MAX_BODY_LEN);
    if (!resp || !body || !resp_header)
        error("Build response");

    func = dispatcher(header->path);

    func(header->path, body, &code);
    response_status_line(code, resp);
    response_header(resp_header);
    resp = realloc(resp, sizeof(char) * (strlen(resp) + strlen(resp_header) +
                                         strlen(body) + 1));
    /* concat into response */
    // TODO use str join
    strcat(resp, resp_header);
    strcat(resp, body);
    free(resp_header);
    free(body);

    return resp;
}

void response_destory(char *resp)
{
    if (resp)
        free(resp);
}

void default_resp(char *args, char *buf, status_code_t *code)
{
    sprintf(buf, "Hello World!");
    *code = OK;
}
