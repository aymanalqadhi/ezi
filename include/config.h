#ifndef EZI_CONFIG_H
#define EZI_CONFIG_H

#include "ds/slist.h"

#include <stddef.h>

enum config_parse_ret
{
    CONFIG_PARSE_SUCCESS       = 0,
    CONFIG_PARSE_INVALID_ARGS  = -1,
    CONFIG_PARSE_UNKNOWN_ERROR = -2,
};

struct ezi_config
{
    const char *exec_name;
    const char *command;

    struct ezi_slist command_args;
};

/*!
 * \brief Parses a command line arguments list into a generic command struct
 *        for the use by the application
 *
 *  \param [out] cfg    A pointer to the output config structure
 *  \param [in]  argc   The command line arguments count
 *  \param [in]  argv   The command line arguments vector
 *
 *  \return 0 for success, a negative value otherwise
 */
int
ezi_config_parse_argv(struct ezi_config *cfg, int argc, char *argv[]);

#endif /* EZI_CONFIG_H */
