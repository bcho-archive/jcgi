#include <stdio.h>

#include "http.h"
#include "request.h"

#define PORT 1024
#define MAXLINE 1024 * 100

int main()
{
    int httpfd;

    httpfd = http_create(PORT, 50);

    while (1) {
        int clientfd;
        size_t limit;
        char buf[MAXLINE], resp[MAXLINE];
        struct request_header *header;

        limit = MAXLINE;
        clientfd = http_accept(httpfd);
        http_read(clientfd, &limit, buf);

        header = request_parse(buf);
        if (request_validate(header)) {
            request_dispatch(header, resp);
        } else {
            sprintf(resp, "HTTP 1.1 400\n");
        }
        request_destory(header);

        http_write(clientfd, resp);
        http_close_client(clientfd);
    }

    http_close(httpfd);

    return 0;
}
