#include <stdio.h>

#include "http.h"
#include "request.h"
#include "response.h"
#include "dev.h"

#define PORT 1024
#define MAXLINE 1024 * 100

int main()
{
    int httpfd;

    httpfd = http_create(PORT, 50);

    while (1) {
        int clientfd;
        size_t limit;
        char buf[MAXLINE];
        char *resp;
        struct request_header *header;

        limit = MAXLINE;
        clientfd = http_accept(httpfd);
        http_read(clientfd, &limit, buf);

        header = request_parse(buf);
        resp = response_build(header, dispatcher);
        http_write(clientfd, resp);

        request_destory(header);
        response_destory(resp);
        http_close_client(clientfd);
    }

    http_close(httpfd);

    return 0;
}
