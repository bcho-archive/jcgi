#ifndef LOGGING_H
#define LOGGING_H

#define LOG_FILE_NAME "server.log"

typedef enum {
    DEBUG,
    NORMAL,
    INFO,
    WARNNING,
    ERROR
} logging_level_t;

void logging_setup(FILE *log_stream, logging_level_t lowest_level,
                   char *log_format);
void logging_destory();
void logging_log(logging_level_t level, char *format, ...);

#endif
