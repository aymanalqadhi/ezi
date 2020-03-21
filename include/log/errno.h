#ifndef EZI_LOG_ERRNO_H
#define EZI_LOG_ERRNO_H

enum ezi_error
{
    EZI_ERR_FIRST_MARKER = 0x1000 - 1,

    /* Global Errors */
    EZI_ERR_NULL_ARGUMENTS,
    EZI_ERR_MEMORY_ALLOC_FAILED,
    EZI_ERR_UNINITIALIZED_DATA,

    /* Configuration Errors */
    EZI_ERR_CONFIG_INVALID_ARGS,
    EZI_ERR_CONFIG_ARGS_LIST_PARSE,

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
