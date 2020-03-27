#include "io/read_write.h"

#include "io/endian.h"
#include "log/errno.h"
#include "util/validation.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CHECK_EOF(fp)                                                          \
    if (feof(fp)) {                                                            \
        errno = EZI_ERR_FILE_EOF_REACHED;                                      \
        return -1;                                                             \
    }

int
ezi_fs_write(const void *buf, size_t len, FILE *fp)
{
    CHECK_NULL_PARAMS_2(buf, fp);
    CHECK_EOF(fp);

    if (fwrite(buf, 1, len, fp) < 0) {
        errno = EZI_ERR_FILE_WRITE_FAILED;
        return -1;
    }

    return 0;
}

int
ezi_fs_write_uint16(uint16_t num, FILE *fp)
{
    CHECK_NULL_PARAMS_1(fp);

    num = ezi_htobe16(num);
    return ezi_fs_write((const void *)&num, sizeof(num), fp);
}

int
ezi_fs_write_uint32(uint32_t num, FILE *fp)
{
    CHECK_NULL_PARAMS_1(fp);

    num = ezi_htobe32(num);
    return ezi_fs_write((const void *)&num, sizeof(num), fp);
}

int
ezi_fs_write_uint64(uint64_t num, FILE *fp)
{
    CHECK_NULL_PARAMS_1(fp);

    num = ezi_htobe64(num);
    return ezi_fs_write((const void *)&num, sizeof(num), fp);
}

int
ezi_fs_write_string(const char *str, FILE *fp)
{
    uint32_t len;

    CHECK_NULL_PARAMS_2(str, fp);

    len = strlen(str);

    return ezi_fs_write_uint32(len, fp) == 0 &&
                   ezi_fs_write((const void *)str, len, fp) == 0
               ? 0
               : -1;
}

int
ezi_fs_read(void *buf, size_t len, FILE *fp)
{
    CHECK_NULL_PARAMS_2(buf, fp);
    CHECK_EOF(fp);

    if (fread(buf, 1, len, fp) < 0) {
        errno = EZI_ERR_FILE_READ_FAILED;
        return -1;
    }

    return 0;
}

int
ezi_fs_read_uint16(uint16_t *nump, FILE *fp)
{
    uint16_t temp;

    CHECK_NULL_PARAMS_2(nump, fp);

    if (ezi_fs_read((void *)&temp, sizeof(temp), fp) != 0) {
        return -1;
    }

    *nump = ezi_betoh16(temp);
    return 0;
}

int
ezi_fs_read_uint32(uint32_t *nump, FILE *fp)
{
    uint32_t temp;

    CHECK_NULL_PARAMS_2(nump, fp);

    if (ezi_fs_read((void *)&temp, sizeof(temp), fp) != 0) {
        return -1;
    }

    *nump = ezi_betoh32(temp);
    return 0;
}

int
ezi_fs_read_uint64(uint64_t *nump, FILE *fp)
{
    uint64_t temp;

    CHECK_NULL_PARAMS_2(nump, fp);

    if (ezi_fs_read((void *)&temp, sizeof(temp), fp) != 0) {
        return -1;
    }

    *nump = ezi_betoh64(temp);
    return 0;
}

int
ezi_fs_read_string(char *buf, size_t *len, FILE *fp)
{
    uint32_t to_read;

    CHECK_NULL_PARAMS_3(buf, len, fp);

    if (ezi_fs_read_uint32(&to_read, fp) != 0) {
        return -1;
    }

    to_read = to_read < *len ? to_read : *len;

    if (ezi_fs_read(buf, to_read, fp) != 0) {
        return -1;
    }

    buf[(*len = to_read)] = '\0';

    return 0;
}
