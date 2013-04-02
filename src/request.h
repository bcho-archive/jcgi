#ifndef REQUEST_H
#define REQUEST_H

struct request_header {
    char *method;
    char *path;
    char *protocol;
};

struct request_header *request_parse(char *request);
int request_validate(struct request_header *header);
void request_dispatch(struct request_header *header, char *resp);
void request_destory(struct request_header *header);

#endif
