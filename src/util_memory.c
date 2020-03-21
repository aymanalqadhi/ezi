#include "util/memory.h"

#include <stdlib.h>
#include <string.h>

void *
ezi_memdup(const void *buf, size_t len)
{
    void *tmp;

    if ((tmp = malloc(len)) == NULL) {
        return NULL;
    }

    return memcpy(tmp, buf, len);
}
