#include "config.h"
#include "log/errno.h"

#include "ds/slist.h"

#include <errno.h>
#include <stddef.h>

int
ezi_config_parse_argv(struct ezi_config *cfg, int argc, char *argv[])
{
    int i;

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
        if (ezi_slist_push(&cfg->command_args, (const void *)(argv + i)) != 0) {
            free_ezi_slist(&cfg->command_args);
            errno = EZI_ERR_CONFIG_ARGS_LIST_PARSE;
            return -1;
        }
    }

    return 0;
}

void
free_ezi_config(struct ezi_config *cfg)
{
    free_ezi_slist(&cfg->command_args);
}
