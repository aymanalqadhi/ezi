#include "config.h"
#include "log/logger.h"
#include "log/errno.h"
#include "util/reflection.h"

#include <errno.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    int               rc;
    struct ezi_config cfg;

    if ((rc = ezi_config_parse_argv(&cfg, argc, argv)) != 0) {
        log_fperror("main", ezi_nameof(ezi_config_parse_argv));
        return 1;
    }

    log_info("main", "Run normally");
    free_ezi_slist(&cfg.command_args);

    return 0;
}
