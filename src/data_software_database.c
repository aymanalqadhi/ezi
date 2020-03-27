#include "data/software_database.h"

#include "data/software.h"
#include "data/software_source.h"
#include "ds/hash_table.h"
#include "util/validation.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DATABASE_TABLE_BUCKETS 0x100

int
init_ezi_software_database(struct ezi_software_database *db,
                           struct ezi_software_source *  installed,
                           struct ezi_software_source *  available)
{
    CHECK_NULL_PARAMS_3(db, installed, available);
    CHECK_UNINITIALIZED_DATA_2(installed->source_fp, available->source_fp);

    db->installed.source = *installed;
    db->available.source = *available;

    if (init_ezi_hash_table(&db->installed.data,
                            DATABASE_TABLE_BUCKETS,
                            EZI_SOFTWARE_MAX_NAME_LEN,
                            sizeof(struct ezi_software)) != 0) {
        return -1;
    }

    if (init_ezi_hash_table(&db->available.data,
                            DATABASE_TABLE_BUCKETS,
                            EZI_SOFTWARE_MAX_NAME_LEN,
                            sizeof(struct ezi_software)) != 0) {
        free_ezi_hash_table(&db->installed.data);
        return -1;
    }

    return 0;
}

int
ezi_software_database_load(struct ezi_software_database *db)
{
    if (ezi_software_source_load(&db->available.source, &db->available.data) ||
        ezi_software_source_load(&db->installed.source, &db->installed.data)) {
        return -1;
    }

    return 0;
}

int
ezi_software_database_save(struct ezi_software_database *db)
{
    if (ezi_software_source_save(&db->available.source, &db->available.data) ||
        ezi_software_source_save(&db->installed.source, &db->installed.data)) {
        return -1;
    }

    return 0;
}

int
ezi_software_table_insert(struct ezi_software_table *tbl,
                          const struct ezi_software *sw)
{
    char namebuf[EZI_SOFTWARE_MAX_NAME_LEN + 1] = { 0 };
    strncpy(namebuf, sw->name, sizeof(namebuf) - 1);

    return ezi_hash_table_set(
        &tbl->data, (const void *)namebuf, (const void *)sw);
}

int
ezi_software_table_get(struct ezi_software_table *tbl,
                       const char *               name,
                       struct ezi_software **     sw)
{
    char namebuf[EZI_SOFTWARE_MAX_NAME_LEN + 1] = { 0 };
    strncpy(namebuf, name, sizeof(namebuf) - 1);

    if (ezi_hash_table_get(&tbl->data, (const void *)namebuf, (void **)sw) !=
        0) {
        return -1;
    }

    return 0;
}

int
ezi_software_table_remove(struct ezi_software_table *tbl, const char *name)
{
    CHECK_NULL_PARAMS_2(tbl, name);
    CHECK_UNINITIALIZED_DATA_1(tbl->data.buckets);

    char namebuf[EZI_SOFTWARE_MAX_NAME_LEN + 1] = { 0 };
    strncpy(namebuf, name, sizeof(namebuf) - 1);

    return ezi_hash_table_remove(&tbl->data, (const void *)namebuf);
}

int
ezi_software_table_load(struct ezi_software_table *tbl)
{
    CHECK_NULL_PARAMS_1(tbl);

    return ezi_software_source_load(&tbl->source, &tbl->data);
}

int
ezi_software_table_save(struct ezi_software_table *tbl)
{
    CHECK_NULL_PARAMS_1(tbl);
    CHECK_UNINITIALIZED_DATA_1(tbl->data.buckets);

    return ezi_software_source_save(&tbl->source, &tbl->data);
}

void
free_ezi_software_table(struct ezi_software_table *tbl)
{
    free_ezi_software_source(&tbl->source);
    free_ezi_hash_table(&tbl->data);
}

void
free_ezi_software_database(struct ezi_software_database *db)
{
    free_ezi_software_table(&db->available);
    free_ezi_software_table(&db->installed);
}
