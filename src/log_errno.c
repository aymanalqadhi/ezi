#include "log/errno.h"

#include <errno.h>
#include <string.h>

#define EZI_ERRORS_COUNT (EZI_ERR_LAST_MARKER - EZI_ERR_FIRST_MARKER - 1)

static const char *ezi_errors[EZI_ERRORS_COUNT] = {
    "NULL arguments were passed",
    "Could not allocate memory",
    "Unintialized data was used",
    "No such command",
    "Command return with an error",
    "Invalid command line arguments",
    "Could not parse arguments list",
    "Initialization failed"
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
