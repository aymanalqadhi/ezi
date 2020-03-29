#include "command.h"
#include "data/software.h"
#include "data/software_database.h"
#include "ds/hash_table.h"
#include "ds/slist.h"
#include "log/colors.h"
#include "log/logger.h"

#include <errno.h>
#include <getopt.h>
#include <string.h>

static inline void
print_info_field(const char *label, const char *value, int newline)
{
    printf(COLORED("%-16s: ", COLOR_CYAN) "%s", label, value);
    if (newline) {
        putchar('\n');
    }
}

static int
list_software_command_exec(const struct ezi_command_ctx *ctx,
                           const struct ezi_slist *      args)
{
    const char *               tbl_name;
    struct ezi_software_table *tbl;

    struct ezi_slist *     bucket;
    struct ezi_slist_node *node, *dep;
    struct ezi_software *  sw;

    if (args->count != 1) {
        goto invalid_args;
    }

    tbl_name = *SLIST_FIRST(args, char **);
    if (strcmp(tbl_name, "installed") == 0) {
        tbl = &ctx->db->installed;
    } else if (strcmp(tbl_name, "available") == 0) {
        tbl = &ctx->db->available;
    } else {
        log_error("list_command", "Invalid table name `%s'", tbl_name);
        goto invalid_args;
    }

    printf("\n- Databse `%s' table:\n", tbl_name);

    HASH_TABLE_ITERATE(&tbl->data, bucket, node)
    {
        sw = (struct ezi_software *)((struct ezi_hash_table_entry *)node->data)
                 ->value;

        print_info_field("Name", sw->name, 1);
        print_info_field("Description", sw->description, 1);
        print_info_field("URL", sw->url, 1);

        if (sw->dependencies.count > 0) {
            print_info_field("Depends On", "", 0);;

            SLIST_ITERATE(&sw->dependencies, dep)
            {
                printf("%s", (char *)dep->data);
                if (dep->next) {
                    printf(", ");
                }
            }

            putchar('\n');
        }

        putchar('\n');
    }

    printf("Total: %lu\n", tbl->data.count);
    return 0;

invalid_args:
    errno = EZI_ERR_COMMAND_INVALID_ARGS;
    return -1;
}

struct ezi_command list_software_command = {
    .display_name = "List Software Command",
    .description  = "Lists a software table contents",
    .usage        = "list <installed|available>",
    .func         = &list_software_command_exec
};
