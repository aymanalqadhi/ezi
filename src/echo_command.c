#include "command.h"
#include "ds/slist.h"
#include "log/logger.h"

static int
echo_command_exec(const struct ezi_command_ctx *ctx,
                  const struct ezi_slist *      args)
{
    char **                arg;
    struct ezi_slist_node *node;
    int                    i;

    i = 0;

    SLIST_ITERATE(args, node)
    {
        arg = (char **)node->data;
        log_norm("echo_command", "Arg #%d: %s", ++i, *arg);
    }

    return 0;
}

struct ezi_command echo_command = { .display_name = "Echo Command",
                                    .description  = "Echoes passed arguments",
                                    .usage        = "ezi echo [args...]",
                                    .func         = &echo_command_exec };
