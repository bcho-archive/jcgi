#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1024
#define MAXLINE 1024

void error(const char *msg)
{
    perror(msg);
    abort();
}

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Open socket failed!");

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
        char *buf, *resp;

        clientfd = accept(sockfd, (struct sockaddr *) &client_addr,
                          &client_addr_len);
        if (clientfd < 0)
            error("Accept failed");
        else
            printf("Connect established\n");

        request_len = read(clientfd, buf, MAXLINE);
        printf("Request length: %d\n", request_len);

        shutdown(clientfd, SHUT_RD);

        resp = "Status: 200\r\nContent-Type: text/plain\r\n";
        write(clientfd, resp, strlen(resp));

        shutdown(clientfd, SHUT_WR);
        close(clientfd);
        printf("Connect close.\n");
    }

    return 0;
}
