#include "util/free.h"

#include <stdlib.h>

void free_string(void *data)
{
    if (data) {
        free(*(char **)data);
    }
}
