#include "io/archive.h"

#include "ds/slist.h"
#include "log/errno.h"
#include "log/logger.h"
#include "util/free.h"
#include "util/validation.h"

#include <archive.h>
#include <archive_entry.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct archive *
open_read_archive(const char *filename);

static struct archive *
open_write_disk_archive(void);

static int
copy_entry_data(struct archive *ar, struct archive *aw);

int
ezi_archive_list_contents(const char *filename, struct ezi_slist *outlist)
{
    char *                item;
    struct archive *      ar;
    struct archive_entry *ent;

    CHECK_NULL_PARAMS_2(filename, outlist);

    if (init_ezi_slist_free(outlist, sizeof(char *), &free_string) != 0) {
        return -1;
    } else if (!(ar = open_read_archive(filename))) {
        errno = EZI_ERR_FILE_OPEN_FAILED;
        free_ezi_slist(outlist);
        return -1;
    }

    while (archive_read_next_header(ar, &ent) == ARCHIVE_OK) {
        if (!(item = strdup(archive_entry_pathname(ent)))) {
            errno = EZI_ERR_MEMORY_ALLOC_FAILED;
            goto error;
        } else if (ezi_slist_push(outlist, (const void *)&item) != 0) {
            goto error;
        }

        archive_read_data_skip(ar);
    }

    archive_free(ar);
    return 0;

error:
    archive_free(ar);
    free_ezi_slist(outlist);
    return -1;
}

int
ezi_archive_extract(const char *filename)
{
    int rc;

    struct archive *      ar;
    struct archive *      ext;
    struct archive_entry *entry;

    if (!(ar = open_read_archive(filename)) ||
        !(ext = open_write_disk_archive())) {
        errno = EZI_ERR_FILE_OPEN_FAILED;
        return -1;
    }

    while (1) {
        if ((rc = archive_read_next_header(ar, &entry)) == ARCHIVE_EOF) {
            break;
        } else if (rc < ARCHIVE_OK) {
            log_warn("archive_extract", archive_error_string(ar));
            if (rc < ARCHIVE_WARN) {
                errno = EZI_ERR_FILE_READ_FAILED;
                return -1;
            }
        }

        if ((rc = archive_write_header(ext, entry)) < ARCHIVE_OK) {
            log_warn("archive_extract", archive_error_string(ar));
        } else if (archive_entry_size(entry) > 0) {
            if ((rc = copy_entry_data(ar, ext)) < ARCHIVE_OK) {
                log_warn("archive_extract", archive_error_string(ar));
            }
            if (rc < ARCHIVE_WARN) {
                errno = EZI_ERR_FILE_WRITE_FAILED;
                return -1;
            }
        }

        if ((rc = archive_write_finish_entry(ext)) < ARCHIVE_OK) {
            log_warn("archive_extract", archive_error_string(ar));
        }
        if (rc < ARCHIVE_WARN) {
            errno = EZI_ERR_FILE_WRITE_FAILED;
            return -1;
        }
    }

    archive_read_close(ar);
    archive_read_free(ar);
    archive_write_close(ext);
    archive_write_free(ext);

    return 0;
}

static struct archive *
open_read_archive(const char *filename)
{
    struct archive *ar;

    if (!(ar = archive_read_new())) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return NULL;
    }

    archive_read_support_filter_all(ar);
    archive_read_support_format_all(ar);
    archive_read_support_compression_all(ar);

    if (archive_read_open_filename(ar, filename, EZI_ARCHIVE_BLOCK_SIZE) !=
        ARCHIVE_OK) {
        archive_read_free(ar);
        return NULL;
    }

    return ar;
}

static struct archive *
open_write_disk_archive(void)
{
    int             flags;
    struct archive *ar;

    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    if (!(ar = archive_write_disk_new())) {
        return NULL;
    }

    archive_write_disk_set_options(ar, flags);
    archive_write_disk_set_standard_lookup(ar);

    return ar;
}

static int
copy_entry_data(struct archive *ar, struct archive *aw)
{
    int         rc;
    size_t      len;
    const void *buf;
    la_int64_t  offset;

    while (1) {
        if ((rc = archive_read_data_block(ar, &buf, &len, &offset)) ==
            ARCHIVE_EOF) {
            return (ARCHIVE_OK);
        } else if (rc < ARCHIVE_OK) {
            return rc;
        }

        if ((rc = archive_write_data_block(aw, buf, len, offset)) <
            ARCHIVE_OK) {
            log_warn("archive_extract", archive_error_string(ar));
            return rc;
        }
    }
}
