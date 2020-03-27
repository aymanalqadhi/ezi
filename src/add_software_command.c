#include "command.h"
#include "data/software.h"
#include "data/software_database.h"
#include "ds/slist.h"
#include "log/logger.h"

#include <getopt.h>
#include <string.h>

#define ADD_SOFTWARE_COMMAND_OPTS "n:u:d:"

static int
add_software_command_exec(const struct ezi_command_ctx *ctx,
                          const struct ezi_slist *      args)
{
    char *              name, *description, *url, arg;
    struct ezi_software sw;

    name = description = url = NULL;

    while ((arg = getopt(ctx->config->argc,
                         ctx->config->argv,
                         ADD_SOFTWARE_COMMAND_OPTS)) != -1) {
        switch (arg) {
        case 'n':
            name = optarg;
            break;

        case 'd':
            description = optarg;
            break;

        case 'u':
            url = optarg;
            break;

        default:
            goto invalid_args;
        }
    }

    if (!name || !url) {
        log_error("add_command", "Name and URL arguments are required");
        goto invalid_args;
    }

    memset(&sw, 0, sizeof(sw));

    strncpy(sw.name, name, sizeof(sw.name));
    strncpy(sw.url, url, sizeof(sw.url));

    if (description) {
        strncpy(sw.description, description, sizeof(sw.description));
    }

    if (ezi_software_table_insert(&ctx->db->available, &sw) != 0 ||
        ezi_software_database_save(ctx->db) != 0) {
        log_error("add_command", "Could not add software `%s'", sw.name);
        return -1;
    }

    log_info("add_command", "Software `%s' was added successfully", sw.name);
    return 0;

invalid_args:
    errno = EZI_ERR_CONFIG_INVALID_ARGS;
    return -1;
}

struct ezi_command add_software_command = {
    .display_name = "Add Software Command",
    .description  = "Adds a software to the available software table",
    .usage        = "ezi add -n name -u url [ -d description ]",
    .func         = &add_software_command_exec
};
