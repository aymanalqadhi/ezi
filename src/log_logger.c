#include "config.h"
#include "debug.h"
#include "log/colors.h"
#include "log/logger.h"

#include <pthread.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct log_level_opts
{
    const char *name;
    uint8_t     color;
} log_level_opts_t;

const static log_level_opts_t log_level_opts[7] = {
    { .name = "fatal", .color = COMBINE_COLOR(COLOR_RED, COLOR_FLAG_BOLD) },
    { .name = "error", .color = COLOR_RED },
    { .name = "warn", .color = COLOR_YELLOW },
    { .name = "info", .color = COLOR_CYAN },
    { .name = "norm", .color = COLOR_MAGENTA },
    { .name = "debug", .color = COLOR_BLUE },
    { .name = "trace", .color = COMBINE_COLOR(0, COLOR_FLAG_BOLD) },
};

void
log_msg(log_level_t level, const char *module, const char *fmt, ...)
{
    time_t     current_time;
    FILE *     log_file;
    char       time_buf[0x20];
    struct tm *tm;
    va_list    vl;

    current_time = time(NULL);
    log_file     = level > LOG_WARN ? stderr : stdout;

    if ((tm = localtime(&current_time)) == NULL ||
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S%z", tm) < 0) {
        strncpy(time_buf, "[0000-00-00 00:00:00+0000]", sizeof(time_buf));
    }

    fprintf(log_file,
            "[%s] [%s] [" COLOR_FORMAT_STR "%s" COLOR_RESET_STR "] ",
            time_buf,
            module,
            GET_COLOR_FLAGS(log_level_opts[level].color),
            GET_COLOR_COLOR(log_level_opts[level].color),
            log_level_opts[level].name);

    va_start(vl, fmt);
    vfprintf(log_file, fmt, vl);
    va_end(vl);

    fputc('\n', log_file);
}
