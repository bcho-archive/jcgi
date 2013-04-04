#include <stdio.h>

#include "logging.h"
#include "http.h"
#include "request.h"
#include "response.h"
#include "dev.h"

#define PORT 1024
#define MAXLINE 1024 * 100

int main()
{
    int httpfd;
    FILE *log;

    log = stdout;
    logging_setup(log, NORMAL, NULL);

    httpfd = http_create(PORT, 50);
    logging_log(NORMAL, "Server started...");

    while (1) {
        int clientfd;
        size_t limit;
        char buf[MAXLINE];
        char *resp;
        struct request_header *header;

        limit = MAXLINE;
        clientfd = http_accept(httpfd);
        http_read(clientfd, &limit, buf);
        logging_log(NORMAL, "Got request from client");

        header = request_parse(buf);
        resp = response_build(header, dispatcher);
        http_write(clientfd, resp);

        request_destory(header);
        response_destory(resp);
        http_close_client(clientfd);
        logging_log(NORMAL, "Responsed");
    }

    http_close(httpfd);
    logging_destory();

    return 0;
}
