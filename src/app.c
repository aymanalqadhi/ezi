#include "app.h"

#include "command.h"
#include "config.h"
#include "ds/hash_table.h"
#include "log/errno.h"
#include "log/logger.h"

#include <errno.h>
#include <string.h>

#define COMMAND_MAX_NAME_LEN 0x20
#define COMMANDS_HT_BUCKETS  0x10

static struct ezi_config     config;
static struct ezi_hash_table commands;

static const struct ezi_command *
get_command(const char *name);

static int
add_command(const char *name, const struct ezi_command *cmd);

int
ezi_app_run(int argc, char *argv[])
{
    static struct ezi_command_ctx command_ctx = { .config      = &config,
                                                  .get_command = &get_command };
    const struct ezi_command *    cmd;

    if (ezi_config_parse_argv(&config, argc, argv) != 0 ||
        init_ezi_hash_table(&commands,
                            COMMANDS_HT_BUCKETS,
                            sizeof(char[COMMAND_MAX_NAME_LEN]),
                            sizeof(struct ezi_command)) != 0) {
        log_perror("app", "ezi_config_parse_argv");
        errno = EZI_ERR_INITIALIZATION_FAILED;
        return -1;
    }

    export_known_commands(&add_command);

    if (!(cmd = get_command(config.command))) {
        errno = EZI_ERR_NO_SUCH_COMMAND;
        return -1;
    }

    if ((*cmd->func)(&command_ctx, &config.command_args) != 0) {
        errno = EZI_ERR_COMMAND_ERROR_RETURN;
        return -1;
    }

    return 0;
}

void
ezi_app_destroy(void)
{
    free_ezi_config(&config);
    free_ezi_hash_table(&commands);
}

static const struct ezi_command *
get_command(const char *name)
{
    char                name_buf[COMMAND_MAX_NAME_LEN] = { 0 };
    struct ezi_command *command;

    strncpy(name_buf, name, sizeof(name_buf));

    if (ezi_hash_table_get(&commands, (void *)name_buf, (void **)&command) !=
        0) {
        log_perror("app", "ezi_hash_table_get");
        return NULL;
    }

    return command;
}

static int
add_command(const char *name, const struct ezi_command *cmd)
{
    char name_buf[COMMAND_MAX_NAME_LEN] = { 0 };

    strncpy(name_buf, name, sizeof(name_buf));
    return ezi_hash_table_set(
        &commands, (const void *)name_buf, (const void *)cmd);
}
