#include "data/software_source.h"

#include "log/errno.h"

#include <stdio.h>
#include <errno.h>

int
init_ezi_software_source_fs(struct ezi_software_source *src,
                            const char *                installed_fn,
                            const char *                available_fn)
{
    if (!(src->installed_fp = fopen(installed_fn, "r+")) ||
        !(src->available_fp = fopen(available_fn, "r+"))) {
        errno = EZI_ERR_SRC_FILES_OPEN_FAILED;
        return -1;
    }

    return 0;
}
