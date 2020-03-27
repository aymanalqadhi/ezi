#ifndef EZI_LOG_ERRNO_H
#define EZI_LOG_ERRNO_H

#include <errno.h>

enum ezi_error
{
    EZI_ERR_FIRST_MARKER = 0x1000 - 1,

    /* Global Errors */
    EZI_ERR_NULL_ARGUMENTS,
    EZI_ERR_MEMORY_ALLOC_FAILED,
    EZI_ERR_UNINITIALIZED_DATA,
    EZI_ERR_NOT_FOUND,

    /* Command Errors */
    EZI_ERR_NO_SUCH_COMMAND,
    EZI_ERR_COMMAND_ERROR_RETURN,

    /* Configuration Errors */
    EZI_ERR_CONFIG_INVALID_ARGS,
    EZI_ERR_CONFIG_ARGS_LIST_PARSE,
    EZI_ERR_INITIALIZATION_FAILED,

    /* Data Errors */
    EZI_ERR_SRC_FILE_OPEN_FAILED,

    /* I/O Errors */
    EZI_ERR_FILE_OPEN_FAILED,
    EZI_ERR_FILE_WRITE_FAILED,
    EZI_ERR_FILE_READ_FAILED,
    EZI_ERR_FILE_EOF_REACHED,

    EZI_ERR_LAST_MARKER
};

/*!
 * \brief Gets the string representation of the current errno
 *
 * \param [int] err  The number of the error
 * \return The string representation of the current errno value
 */
const char *
ezi_strerror(int err);

#endif /* EZI_LOG_ERRNO_H */
