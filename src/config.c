#include "config.h"

#include "debug.h"
#include "ds/slist.h"

#include <stddef.h>

int
ezi_config_parse_argv(struct ezi_config *cfg,
                      int                argc,
                      char *argv[])
{
    int i;

    massert(cfg != NULL, "Config structure ponter cannot be NULL");
    massert(argv != NULL, "Argument vector cannot be NULL");

    if (argc < 2) {
        return CONFIG_PARSE_INVALID_ARGS;
    }

    if (init_ezi_slist(&cfg->command_args, sizeof(char *)) != 0) {
        return CONFIG_PARSE_UNKNOWN_ERROR;
    }

    cfg->exec_name = argv[0];
    cfg->command   = argv[1];

    argc -= 2;
    argv += 2;
    i = 0;

    for (i = 0; argc--; ++i) {
        if (ezi_slist_push(&cfg->command_args, (const void *)(argv + i)) == NULL) {
            free_ezi_slist(&cfg->command_args);
            return CONFIG_PARSE_UNKNOWN_ERROR;
        }
    }

    return CONFIG_PARSE_SUCCESS;
}
