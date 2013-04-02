#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "http.h"
#include "error.h"

int http_create(uint16_t port, int backlog)
{
    int sockfd, opt;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("HTTP Create");
    opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)))
        error("Bind");

    if (listen(sockfd, backlog) < 0)
        error("Listen");

    return sockfd;
}

int http_accept(int serverfd)
{
    struct sockaddr addr;
    socklen_t addr_len;
    int clientfd;

    clientfd = accept(serverfd, (struct sockaddr *) &addr, &addr_len);

    if (clientfd < 0)
        error("Accept");

    return clientfd;
}

void http_read(int clientfd, size_t *limit, char *buf)
{
    size_t length;

    length = read(clientfd, buf, (int) *limit);
    buf[length] = 0;
    shutdown(clientfd, SHUT_RD);

    *limit = length;
}

void http_write(int clientfd, char *resp)
{
    write(clientfd, resp, strlen(resp));
    shutdown(clientfd, SHUT_WR);
}

void http_close(int httpfd)
{
    close(httpfd);
}

void http_close_client(int clientfd)
{
    close(clientfd);
}
