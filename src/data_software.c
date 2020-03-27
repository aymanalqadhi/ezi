#include "data/software.h"

#include "ds/hash_table.h"
#include "ds/slist.h"
#include "io/read_write.h"
#include "util/validation.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DATABASE_TABLE_BUCKETS 0x100

int
ezi_software_read(struct ezi_software *sw, FILE *fp)
{
    size_t   name_len, desc_len, url_len;
    uint16_t ndeps;
    char     dep_name[sizeof(sw->name)];

    CHECK_NULL_PARAMS_2(sw, fp);

    name_len = sizeof(sw->name);
    desc_len = sizeof(sw->description);
    url_len  = sizeof(sw->url);

    memset((void *)sw, 0, sizeof(*sw));

    if (ezi_fs_read_string(sw->name, &name_len, fp) != 0 ||
        ezi_fs_read_string(sw->description, &desc_len, fp) != 0 ||
        ezi_fs_read_string(sw->url, &url_len, fp) != 0 ||
        ezi_fs_read_uint16(&ndeps, fp) != 0 ||
        init_ezi_slist(&sw->dependencies, sizeof(dep_name)) != 0) {
        errno = EZI_ERR_FILE_READ_FAILED;
        return -1;
    }

    while (ndeps--) {
        name_len = sizeof(dep_name);
        memset(dep_name, 0, sizeof(dep_name));

        if (ezi_fs_read_string(dep_name, &name_len, fp) != 0 ||
            ezi_slist_shift(&sw->dependencies, (const void *)&dep_name) != 0) {
            free_ezi_software(sw);
            errno = EZI_ERR_FILE_READ_FAILED;
            return -1;
        }
    }

    return 0;
}

int
ezi_software_write(const struct ezi_software *sw, FILE *fp)
{
    struct ezi_slist_node *node;
    struct ezi_software *  dep;

    CHECK_NULL_PARAMS_2(sw, fp);

    if (ezi_fs_write_string(sw->name, fp) != 0 ||
        ezi_fs_write_string(sw->description, fp) != 0 ||
        ezi_fs_write_string(sw->url, fp) != 0 ||
        ezi_fs_write_uint16(sw->dependencies.count, fp) != 0) {
        errno = EZI_ERR_FILE_WRITE_FAILED;
        return -1;
    }

    SLIST_ITERATE(&sw->dependencies, node)
    {
        dep = (struct ezi_software *)node->data;
        if (ezi_fs_write_string(dep->name, fp) != 0) {
            errno = EZI_ERR_FILE_WRITE_FAILED;
            return -1;
        }
    }

    return 0;
}
int
ezi_software_decode(struct ezi_software *sw, const void *buf, size_t *len)
{
    FILE *fmemp;

    CHECK_NULL_PARAMS_2(sw, buf);

    if (!(fmemp = fmemopen((void *)buf, *len, "r"))) {
        errno = EZI_ERR_SRC_FILE_OPEN_FAILED;
        return -1;
    }

    if (ezi_software_read(sw, fmemp) != 0) {
        fclose(fmemp);
        return -1;
    }

    *len = ftell(fmemp);
    fclose(fmemp);

    return 0;
}

int
ezi_software_encode(const struct ezi_software *sw, void *buf, size_t *len)
{
    FILE *fmemp;

    CHECK_NULL_PARAMS_3(sw, buf, len);
    CHECK_UNINITIALIZED_DATA_3(sw->name, sw->description, sw->url);

    if (!(fmemp = fmemopen(buf, *len, "w"))) {
        errno = EZI_ERR_SRC_FILE_OPEN_FAILED;
        return -1;
    }

    if (ezi_software_write(sw, fmemp) != 0) {
        fclose(fmemp);
        return -1;
    }

    *len = ftell(fmemp);
    fclose(fmemp);

    return 0;
}

void
free_ezi_software(struct ezi_software *sw)
{
    free_ezi_slist(&sw->dependencies);
}
