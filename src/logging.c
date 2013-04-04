#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"

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

void logging_log(logging_level_t level, char *msg)
{
    if (level < LOGGER.lowest_level)
        return;
    fprintf(LOGGER.log_stream, LOGGER.log_format, level_str(level), msg);
    fflush(LOGGER.log_stream);
}
