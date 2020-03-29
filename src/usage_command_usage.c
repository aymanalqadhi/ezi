#include "command.h"
#include "data/software.h"
#include "data/software_database.h"
#include "io/http.h"
#include "log/colors.h"
#include "log/errno.h"
#include "log/logger.h"

#include <errno.h>
#include <string.h>

#define MAX_FILE_NAME_LEN 0x40

static int
show_usage_command_exec(const struct ezi_command_ctx *ctx,
                        const struct ezi_slist *      args)
{
    const struct ezi_command *cmd;

    if (args->count != 1) {
        errno = EZI_ERR_COMMAND_INVALID_ARGS;
        return -1;
    }

    if (!(cmd = ctx->get_command(*(char **)args->head->data))) {
        log_error("usage_command",
                  "No such command: %s",
                  *(char **)args->head->data);
        return -1;
    }

    printf(COLORED("\n%s Usage", COLOR_BCYAN) ": %s\n",
           cmd->display_name,
           cmd->usage);
    return 0;
}

struct ezi_command show_usage_command = {
    .display_name = "Show Usage Command",
    .description  = "Show the usage syntax of a command",
    .usage        = "usage <command-name>",
    .func         = &show_usage_command_exec
};
