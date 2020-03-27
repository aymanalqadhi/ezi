#include "command.h"
#include "data/software.h"
#include "data/software_database.h"
#include "log/logger.h"

#include <errno.h>
#include <getopt.h>
#include <string.h>

#define ADD_SOFTWARE_COMMAND_OPTS "n:u:d:D:"

static int
add_software_command_exec(const struct ezi_command_ctx *ctx,
                          const struct ezi_slist *      args)
{
    char *              name, *description, *url, *deps, arg;
    char *              tok, *ptr, depbuf[EZI_SOFTWARE_MAX_NAME_LEN];
    struct ezi_software sw;

    name = description = url = deps = NULL;

    while ((arg = getopt(ctx->config->argc,
                         ctx->config->argv,
                         ADD_SOFTWARE_COMMAND_OPTS)) != -1) {
        switch (arg) {
        case 'n':
            name = optarg;
            break;

        case 'u':
            url = optarg;
            break;

        case 'd':
            description = optarg;
            break;

        case 'D':
            deps = optarg;
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

    if (init_ezi_slist(&sw.dependencies, EZI_SOFTWARE_MAX_NAME_LEN) != 0) {
        return -1;
    }

    if (deps) {
        ptr = deps;
        while ((tok = strtok(ptr, ",")) != 0) {
            memset(depbuf, 0, sizeof(depbuf));
            strncpy(depbuf, tok, sizeof(depbuf));

            if (ezi_slist_push(&sw.dependencies, (const void *)depbuf) != 0) {
                return -1;
            }

            ptr = NULL;
        }
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
