#include "util/path.h"

#include "log/errno.h"
#include "util/validation.h"

#include <regex.h>
#include <string.h>

#define EZI_PATH_PATTERN "[^\\/]+$"
#define ARRLEN(arr)      (sizeof(arr) / sizeof(arr[0]))

int
ezi_base_filename(const char *str, size_t len, char *out, size_t outlen)
{
    regex_t    patt;
    regmatch_t fn_group;
    size_t     match_len;

    CHECK_NULL_PARAMS_2(str, out);

    if (regcomp(&patt, EZI_PATH_PATTERN, REG_EXTENDED | REG_ICASE) != 0) {
        errno = EZI_ERR_REGEX_COMPILE_FAILED;
        return -1;
    }

    if (regexec(&patt, str, 1, &fn_group, 0) != 0) {
        errno = EZI_ERR_REGEX_NO_MATCH;
        regfree(&patt);
        return -1;
    }

    match_len = fn_group.rm_eo - fn_group.rm_so;
    match_len = match_len > outlen ? outlen : match_len;

    memcpy(out, str + fn_group.rm_so, match_len);
    out[match_len] = '\0';

    regfree(&patt);
    return 0;
}

int
ezi_base_filename_no_ext(const char *str, size_t len, char *out, size_t outlen)
{
    int i;

    if (ezi_base_filename(str, len, out, outlen) != 0) {
        return -1;
    }

    i = strnlen(out, outlen);
    while (i > 0 && out[i] != '.')
        --i;

    if (i > 0) {
        memset(out + i, 0, outlen - i);
    }

    return 0;
}
