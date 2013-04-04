#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "logging.h"

#define MAX_LENGTH 1024

struct logger {
    FILE *log_stream;
    char *log_format;
    logging_level_t lowest_level;
} LOGGER;

static char *level_str(logging_level_t level)
{
    if (level == DEBUG)
        return "DEBUG";
    if (level == NORMAL)
        return "NORMAL";
    if (level == INFO)
        return "INFO";
    if (level == WARNNING)
        return "WARNNING";
    if (level == ERROR)
        return "ERROR";
}

void logging_setup(FILE *log_stream, logging_level_t lowest_level,
                   char *log_format)
{
    LOGGER.log_stream = log_stream;
    LOGGER.lowest_level = lowest_level;
    if (log_format != NULL) {
        LOGGER.log_format = malloc(sizeof(char) * strlen(log_format));
        strcpy(LOGGER.log_format, log_format);
    } else {
        LOGGER.log_format = "%s: %s\n";
    }
}

void logging_destory()
{
    fclose(LOGGER.log_stream);
}

void logging_log(logging_level_t level, char *format, ...)
{
    char msg[MAX_LENGTH];
    va_list list;

    if (level < LOGGER.lowest_level)
        return;
    sprintf(msg, LOGGER.log_format, level_str(level), format);
    va_start(list, format);
    vfprintf(LOGGER.log_stream, msg, list);
    va_end(list);
    fflush(LOGGER.log_stream);
}
