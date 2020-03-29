#include "command.h"
#include "data/software.h"
#include "data/software_database.h"
#include "io/archive.h"
#include "io/http.h"
#include "log/errno.h"
#include "log/logger.h"
#include "util/path.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#define MAX_FILE_NAME_LEN 0x40

static inline int
download_software_file(const char *url, size_t len, char *fn, size_t fnlen);

static const char download_prefix[] = "/tmp/";

static int
install_software_command_exec(const struct ezi_command_ctx *ctx,
                              const struct ezi_slist *      args)
{
    struct ezi_software *sw;
    char                 current_dir[FILENAME_MAX], buf[FILENAME_MAX];

    if (args->count != 1) {
        errno = EZI_ERR_COMMAND_INVALID_ARGS;
        log_perror("install_command", "install_command");
        return -1;
    }

    if (ezi_software_table_get(
            &ctx->db->available, *(char **)args->head->data, &sw) != 0) {
        log_error("install_command",
                  "No entry with the name `%s' was found",
                  *(char **)args->head->data);
        return -1;
    }

    if (!getcwd(current_dir, sizeof(current_dir))) {
        log_error("install_command", "Could not get current working directory");
        return -1;
    } else if (chdir(download_prefix) != 0) {
        log_error("install_command", "Could not change working directory");
        return -1;
    }

    if (ezi_base_filename_no_ext(sw->url, sizeof(sw->url), buf, sizeof(buf)) !=
            0 ||
        mkdir(buf, 0755) != 0 || chdir(buf) != 0) {
        return -1;
    }

    if (download_software_file(sw->url, sizeof(sw->url), buf, sizeof(buf)) !=
        0) {
        log_perror("install_command", "download_software_file");
        chdir(current_dir);
        return -1;
    }

    if (ezi_archive_extract(buf) != 0) {
        return -1;
    }

    if (system("make") || system("make install")) {
        log_error("install_command", "Could not install software");
        chdir(current_dir);
        return -1;
    }

    chdir(current_dir);
    return 0;
}

static inline int
download_software_file(const char *url, size_t urllen, char *fn, size_t fnlen)
{
    if (ezi_base_filename(url, urllen, fn, fnlen) != 0) {
        return -1;
    }

    log_info("install_command", "Downloading `%s'", fn);
    if (ezi_http_download(url, fn, &ezi_http_download_progress) != 0) {
        return -1;
    }

    return 0;
}

struct ezi_command install_software_command = {
    .display_name = "Install Software Command",
    .description  = "Install a software from the available software table",
    .usage        = "install <name>",
    .func         = &install_software_command_exec
};
