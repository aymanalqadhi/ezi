#ifndef EZI_CONFIG_H
#define EZI_CONFIG_H

#include "ds/slist.h"

#include <stddef.h>
#include <stdint.h>

#define EZI_CONFIG_INSTALLED_TABLE  ".ezi-installed-%04d.db"
#define EZI_CONFIG_AVAILABLE_TABLE  ".ezi-available-%04d.db"
#define EZI_CONFIG_DATABASE_VERSION 1

struct ezi_config
{
    int    argc;
    char **argv;

    const char *command;
    const char *installed_table_fmt;
    const char *available_table_fmt;

    uint32_t database_version;

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
 *  \return 0 for success, a -1 otherwise
 */
int
ezi_config_parse_argv(struct ezi_config *cfg, int argc, char *argv[]);

/*!
 * \brief Frees resources used by the configuration object
 * \param [in] cfg  A pointer to the configuration object to free
 */
void
free_ezi_config(struct ezi_config *cfg);

#endif /* EZI_CONFIG_H */
