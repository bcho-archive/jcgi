#ifndef HTTP_H
#define HTTP_H

#include <stdint.h>

int http_create(uint16_t port, int backlog);
int http_accept(int serverfd);
void http_read(int clientfd, size_t *limit, char *buf);
void http_write(int clientfd, char *resp);
void http_close(int httpfd);
void http_close_client(int clientfd);

#endif
