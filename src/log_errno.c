#include "log/errno.h"

#include <errno.h>
#include <string.h>

static const char *ezi_errors[EZI_ERR_LAST_MARKER - 2] = 
{
    "Invalid command line arguments",
    "Could not parse arguments list"
};

const char *
ezi_strerror(int err)
{
    if (err < EZI_ERR_FIRST_MARKER) {
        return strerror(err);
    } else if (err < EZI_ERR_LAST_MARKER) {
        return ezi_errors[err - EZI_ERR_FIRST_MARKER - 1];
    }

    return NULL;
}
