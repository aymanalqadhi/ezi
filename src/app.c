#include "app.h"

#include "config.h"
#include "log/errno.h"
#include "log/logger.h"
#include "util/reflection.h"

#include <errno.h>

static struct ezi_config config;

int
ezi_app_run(int argc, char *argv[])
{
    if (ezi_config_parse_argv(&config, argc, argv) != 0) {
        log_perror("app", ezi_nameof(ezi_config_parse_argv));
        errno = EZI_ERR_INITIALIZATION_FAILED;
        return -1;
    }

    log_info("app", "Run normally");

    return 0;
}

void
ezi_app_destroy(void)
{
    free_ezi_config(&config);
}
