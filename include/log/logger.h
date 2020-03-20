#ifndef EZI_LOG_LOGGER_H
#define EZI_LOG_LOGGER_H

#include "config.h"

#include <stdint.h>
#include <stdio.h>

typedef enum
{
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_NORM,
    LOG_DEBUG,
    LOG_TRACE
} log_level_t;

/*!
 * \brief Logs a message with a specific log level using the default logger
 * \param[in] level  The log message level
 * \param[in] module The module on which this log occured
 * \param[in] fmt    The log message format
 */
void
log_msg(log_level_t level, const char *module, const char *fmt, ...);

#define log_fatal(m, fmt, ...) log_msg(LOG_FATAL, m, fmt, ##__VA_ARGS__)
#define log_error(m, fmt, ...) log_msg(LOG_ERROR, m, fmt, ##__VA_ARGS__)
#define log_warn(m, fmt, ...)  log_msg(LOG_WARN, m, fmt, ##__VA_ARGS__)
#define log_info(m, fmt, ...)  log_msg(LOG_INFO, m, fmt, ##__VA_ARGS__)
#define log_norm(m, fmt, ...)  log_msg(LOG_NORM, m, fmt, ##__VA_ARGS__)
#define log_debug(m, fmt, ...) log_msg(LOG_DEBUG, m, fmt, ##__VA_ARGS__)
#define log_trace(m, fmt, ...) log_msg(LOG_TRACE, m, fmt, ##__VA_ARGS__)

#endif /* EZI_LOG_LOGGER_H */
