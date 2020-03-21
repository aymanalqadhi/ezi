#ifndef EZI_COMMAND_H
#define EZI_COMMAND_H

#include "ds/slist.h"

typedef int (*command_func)(const struct ezi_slist *);

struct ezi_command
{
    const char *name;
    const char *description;
    const char *usage;

    command_func func;
};

#endif /* EZI_COMMAND_H */
