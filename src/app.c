#include "app.h"

#include "command.h"
#include "config.h"
#include "data/software_database.h"
#include "data/software_source.h"
#include "ds/hash_table.h"
#include "log/errno.h"
#include "log/logger.h"

#include <errno.h>
#include <string.h>

#define COMMAND_MAX_NAME_LEN 0x20
#define COMMANDS_HT_BUCKETS  0x10

static struct ezi_config            config;
static struct ezi_hash_table        commands;
static struct ezi_software_database db;

static const struct ezi_command *
get_command(const char *name);

static int
add_command(const char *name, const struct ezi_command *cmd);

static int
init_app(int argc, char *argv[]);

int
ezi_app_run(int argc, char *argv[])
{
    static struct ezi_command_ctx command_ctx = { .config      = &config,
                                                  .db          = &db,
                                                  .get_command = &get_command };
    const struct ezi_command *    cmd;

    if (init_app(argc, argv) != 0) {
        return -1;
    }

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
    free_ezi_software_database(&db);
}

static int
init_app(int argc, char *argv[])
{
    struct ezi_software_source isrc, asrc;
    char                       fnbuf[0x100];

    if (ezi_config_parse_argv(&config, argc, argv) != 0 ||
        init_ezi_hash_table(&commands,
                            COMMANDS_HT_BUCKETS,
                            sizeof(char[COMMAND_MAX_NAME_LEN]),
                            sizeof(struct ezi_command)) != 0) {
        return -1;
    }

    log_debug("app", "Opening installed software table");
    snprintf(fnbuf,
             sizeof(fnbuf),
             config.installed_table_fmt,
             config.database_version);
    if (init_ezi_software_source_fs(&isrc, fnbuf, config.database_version) !=
        0) {
        return -1;
    }

    log_debug("app", "Opening available software table");
    snprintf(fnbuf,
             sizeof(fnbuf),
             config.available_table_fmt,
             config.database_version);
    if (init_ezi_software_source_fs(&asrc, fnbuf, config.database_version) !=
        0) {
        return -1;
    }

    log_debug("app", "Creating a database object");
    if (init_ezi_software_database(&db, &isrc, &asrc) != 0) {
        return -1;
    }

    log_norm("app", "Loading the software database");
    if (ezi_software_database_load(&db) != 0) {
        return -1;
    }

    export_known_commands(&add_command);
    return 0;
}

static const struct ezi_command *
get_command(const char *name)
{
    char                name_buf[COMMAND_MAX_NAME_LEN + 1] = { 0 };
    struct ezi_command *command;

    strncpy(name_buf, name, sizeof(name_buf) - 1);

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
    char name_buf[COMMAND_MAX_NAME_LEN + 1] = { 0 };

    strncpy(name_buf, name, sizeof(name_buf) - 1);
    return ezi_hash_table_set(
        &commands, (const void *)name_buf, (const void *)cmd);
}
