#include "app.h"
#include "log/logger.h"

#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int ret_code;

    ret_code = EXIT_SUCCESS;

    if (ezi_app_run(argc, argv) != 0) {
        log_fperror("main", "ezi_app_run");
        ret_code = EXIT_FAILURE;
    }

    ezi_app_destroy();
    return ret_code;
}
