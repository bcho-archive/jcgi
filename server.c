#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "request.h"

#define PORT 1024
#define MAXLINE 1024 * 100

char *ok_tmpl =
   "HTTP/1.1 200 OK\n"
   "Content-Type: text/json \n"
   "\n"
   "{name: \"world\"}";

char *bad_request =
    "HTTP/1.1 400 Bad Request\n"
    "Content-Type: text/plain \n"
    "\n"
    "<html><body><h1>Bad Request</h1</body></html>\n";

void error(const char *msg)
{
    perror(msg);
    abort();
}

int main()
{
    int sockfd, opt;
    struct sockaddr_in server_addr, client_addr;
    struct request_header *reqs;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Open socket failed!");
    opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)))
        error("Bind failed!");

    if (listen(sockfd, 50) < 0)
        error("Listen failed!");
    else
        printf("Start listening on 0.0.0.0:%d\n", PORT);

    while (1) {
        socklen_t client_addr_len;
        int clientfd;
        ssize_t request_len;
        char buf[MAXLINE], resp[MAXLINE];

        clientfd = accept(sockfd, (struct sockaddr *) &client_addr,
                          &client_addr_len);
        if (clientfd < 0)
            error("Accept failed");
        else
            printf("Connect established\n");

        request_len = read(clientfd, buf, MAXLINE);
        buf[request_len] = 0;

        shutdown(clientfd, SHUT_RD);

        reqs = request_parse(buf);
        if (request_validate(reqs)) {
            request_dispatch(reqs, resp);
        } else {
            sprintf(resp, "%s", bad_request);
        }
        request_destory(reqs);

        write(clientfd, resp, strlen(resp));

        shutdown(clientfd, SHUT_WR);
        close(clientfd);
        printf("Connect close.\n");
    }

    return 0;
}
