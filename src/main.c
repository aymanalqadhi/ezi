#include "config.h"

#include <stdio.h>

int
main(int argc, char *argv[])
{
    int               rc;
    struct ezi_config cfg;

    if ((rc = ezi_config_parse_argv(&cfg, argc, argv)) !=
        CONFIG_PARSE_SUCCESS) {
        switch (rc) {
        case CONFIG_PARSE_INVALID_ARGS:
            fprintf(stderr, "[!] Invalid arguments\n");
            break;
        case CONFIG_PARSE_UNKNOWN_ERROR:
            fprintf(stderr, "[!] Could not parse arguments\n");
            break;
        default:
            fprintf(stderr,
                    "[!] Unknown error occured while parsing arguments\n");
            break;
        }
    }

    free_ezi_slist(&cfg.command_args);

    return 0;
}
