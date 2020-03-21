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

const static char *level_labels[7] = {
    COLORED("fatal", COLOR_BRED),
    COLORED("error", COLOR_RED),
    COLORED("warn", COLOR_YELLOW),
    COLORED("info", COLOR_CYAN),
    COLORED("norm", COLOR_MAGENTA),
    COLORED("debug", COLOR_BLUE),
    "trace",
};

void
log_msg(log_level_t level, const char *module, const char *fmt, ...)
{
    time_t     current_time;
    FILE *     log_file;
    char       time_buf[0x20];
    va_list    vl;
    struct tm *tm;

    current_time = time(NULL);
    log_file     = level > LOG_WARN ? stderr : stdout;

    if ((tm = localtime(&current_time)) == NULL ||
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S%z", tm) < 0) {
        strncpy(time_buf, "[0000-00-00 00:00:00+0000]", sizeof(time_buf));
    }

    fprintf(log_file, "[%s] [%s] [%s] ",time_buf, module, level_labels[level]);

    va_start(vl, fmt);
    vfprintf(log_file, fmt, vl);
    va_end(vl);

    fputc('\n', log_file);
}
