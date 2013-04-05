#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "response.h"
#include "http_header.h"
#include "error.h"
#include "logging.h"

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

static void response_header(char *header, struct http_header *fields)
{
    char buf[MAX_HEADER_LEN];

    while (fields != NULL) {
        sprintf(buf, "%s:%s\n", fields->name, fields->value);
        strcat(header, buf);
        fields = fields->next;
    }
    strcat(header, "\r\n");
}

char *response_build(const struct request_header *rq_header,
                     dispatcher_t dispatcher)
{
    resp_t func;
    char *rp, *rp_status_line, *rp_header, *rp_body;
    status_code_t code;
    struct http_header *http_header;
    char body_length[20], content_type[20];

    rp_status_line = malloc(sizeof(char) * MAX_STATUS_LINE_LEN);
    rp_header = malloc(sizeof(char) * MAX_HEADER_LEN);
    rp_body = malloc(sizeof(char) * MAX_BODY_LEN);
    if (!rp_status_line || !rp_header || !rp_body)
        error("Build response");

    /* generate response body */
    func = dispatcher(rq_header->path);
    func(rq_header->path, rp_body, &code, content_type);
    logging_log(NORMAL, "Response <%d %s>", code, content_type);

    /* generate header */
    response_status_line(code, rp_status_line);
    rp_header[0] = 0;
    http_header = header_create("Server", SERVERNAME "/" VERSION);
    sprintf(body_length, "%d", strlen(rp_body));
    header_append(http_header, header_create("Content-Length", body_length));
    header_append(http_header, header_create("Content-Type", content_type));
    response_header(rp_header, http_header);
    header_destory(http_header);

    /* concat into response */
    // TODO use str join
    rp = malloc(sizeof(char) * (strlen(rp_status_line) + strlen(rp_header) +
                                strlen(rp_body) + 1));
    sprintf(rp, "%s%s%s", rp_status_line, rp_header, rp_body);
    free(rp_status_line);
    free(rp_header);
    free(rp_body);

    return rp;
}

void response_destory(char *resp)
{
    if (resp)
        free(resp);
}

void default_resp(char *args, char *buf, status_code_t *code,
                  char *content_type)
{
    sprintf(buf, "Hello World!");
    *code = OK;
    sprintf(content_type, "text/plain");
}
