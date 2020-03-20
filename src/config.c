#include "config.h"
#include "log/errno.h"

#include "debug.h"
#include "ds/slist.h"

#include <errno.h>
#include <stddef.h>

int
ezi_config_parse_argv(struct ezi_config *cfg, int argc, char *argv[])
{
    int i;

    massert(cfg != NULL, "Config structure ponter cannot be NULL");
    massert(argv != NULL, "Argument vector cannot be NULL");

    if (argc < 2) {
        errno = EZI_ERR_CONFIG_INVALID_ARGS;
        return -1;
    }

    if (init_ezi_slist(&cfg->command_args, sizeof(char *)) != 0) {
        errno = EZI_ERR_CONFIG_ARGS_LIST_PARSE;
        return -1;
    }

    cfg->exec_name = argv[0];
    cfg->command   = argv[1];

    argc -= 2;
    argv += 2;

    for (i = 0; argc--; ++i) {
        if (ezi_slist_push(&cfg->command_args, (const void *)(argv + i)) ==
            NULL) {
            free_ezi_slist(&cfg->command_args);

            errno = EZI_ERR_CONFIG_ARGS_LIST_PARSE;
            return -1;
        }
    }

    return 0;
}
