#include "data/software_source.h"

#include "data/software.h"
#include "io/read_write.h"
#include "log/errno.h"
#include "util/validation.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define DATA_SOURCE_BUCKETS         0x100
#define DATA_SOURCE_DEFAULT_VERSION 0

static inline int
read_source_metadata(struct ezi_software_source *src);

static inline int
write_source_metadata(struct ezi_software_source *src);

int
init_ezi_software_source_fs(struct ezi_software_source *src,
                            const char *                filename,
                            uint32_t                    version)
{
    CHECK_NULL_PARAMS_2(src, filename);

    if (!(src->source_fp = fopen(filename, "r+"))) {
        if (!(src->source_fp = fopen(filename, "w+"))) {
            errno = EZI_ERR_SRC_FILE_OPEN_FAILED;
            return -1;
        }

        src->metadata.version   = version;
        src->metadata.checksum  = src->metadata.last_update =
            src->metadata.count = 0;

        if (write_source_metadata(src)) {
            errno = EZI_ERR_FILE_WRITE_FAILED;
            return -1;
        }

        rewind(src->source_fp);
    }

    return errno = 0;
}

int
init_ezi_software_source_ms(struct ezi_software_source *src,
                            void *                      buf,
                            size_t                      len,
                            uint32_t                    version)
{
    CHECK_NULL_PARAMS_2(src, buf);

    if (!(src->source_fp = fmemopen(buf, len, "r+"))) {
        errno = EZI_ERR_SRC_FILE_OPEN_FAILED;
        return -1;
    }

    src->metadata.version  = version;
    src->metadata.checksum = src->metadata.last_update = 0;

    return 0;
}

int
ezi_software_source_load(struct ezi_software_source *src,
                         struct ezi_hash_table *     ht)
{
    uint32_t            count;
    struct ezi_software sw;

    CHECK_NULL_PARAMS_2(src, ht);
    CHECK_UNINITIALIZED_DATA_1(src->source_fp);

    if (read_source_metadata(src) != 0) {
        errno = EZI_ERR_FILE_READ_FAILED;
        return -1;
    }

    count = src->metadata.count;
    ezi_hash_table_clear(ht);

    while (count-- > 0 && !feof(src->source_fp)) {
        if (ezi_software_read(&sw, src->source_fp) != 0 ||
            ezi_hash_table_set(ht, (const void *)sw.name, (const void *)&sw) !=
                0) {
            free_ezi_hash_table(ht);
            return -1;
        }
    }

    rewind(src->source_fp);
    return 0;
}

int
ezi_software_source_save(struct ezi_software_source *src,
                         struct ezi_hash_table *     ht)
{
    uint32_t                     count;
    struct ezi_slist *           bucket;
    struct ezi_slist_node *      node;
    struct ezi_hash_table_entry *entry;

    CHECK_NULL_PARAMS_2(src, ht);
    CHECK_UNINITIALIZED_DATA_2(src->source_fp, ht->buckets);

    src->metadata.count = count = ht->count;

    if (ftruncate(fileno(src->source_fp), 0) != 0 ||
        write_source_metadata(src) != 0) {
        errno = EZI_ERR_FILE_WRITE_FAILED;
        return -1;
    }

    if (count > 0) {
        HASH_TABLE_ITERATE(ht, bucket, node)
        {
            if (count-- == 0) {
                break;
            }

            entry = (struct ezi_hash_table_entry *)node->data;
            if (ezi_software_write((struct ezi_software *)entry->value,
                                   src->source_fp) != 0) {
                return -1;
            }
        }
    }

    rewind(src->source_fp);
    return 0;
}

void
free_ezi_software_source(struct ezi_software_source *src)
{
    fclose(src->source_fp);
    src->source_fp = NULL;
}

static inline int
read_source_metadata(struct ezi_software_source *src)
{
    CHECK_NULL_PARAMS_1(src);

    if (ezi_fs_read_uint32(&src->metadata.version, src->source_fp) != 0 ||
        ezi_fs_read_uint32(&src->metadata.checksum, src->source_fp) != 0 ||
        ezi_fs_read_uint64(&src->metadata.last_update, src->source_fp) != 0 ||
        ezi_fs_read_uint32(&src->metadata.count, src->source_fp) != 0) {
        return -1;
    }

    return 0;
}

static inline int
write_source_metadata(struct ezi_software_source *src)
{
    CHECK_NULL_PARAMS_1(src);

    if (ezi_fs_write_uint32(src->metadata.version, src->source_fp) != 0 ||
        ezi_fs_write_uint32(src->metadata.checksum, src->source_fp) != 0 ||
        ezi_fs_write_uint64(src->metadata.last_update, src->source_fp) != 0 ||
        ezi_fs_write_uint32(src->metadata.count, src->source_fp) != 0) {
        return -1;
    }

    return 0;
}
