#ifndef EZI_UTIL_VALIDATION_H
#define EZI_UTIL_VALIDATION_H

#define CHECK_NULL_PARAMS_1(p1)                                                \
    if (!(p1)) {                                                               \
        errno = EZI_ERR_NULL_ARGUMENTS;                                        \
        return -1;                                                             \
    }

#define CHECK_NULL_PARAMS_2(p1, p2)                                            \
    if (!(p1) || !(p2)) {                                                      \
        errno = EZI_ERR_NULL_ARGUMENTS;                                        \
        return -1;                                                             \
    }

#define CHECK_NULL_PARAMS_3(p1, p2, p3)                                        \
    if (!(p1) || !(p2) || !(p3)) {                                             \
        errno = EZI_ERR_NULL_ARGUMENTS;                                        \
        return -1;                                                             \
    }

#define CHECK_NULL_PARAMS_4(p1, p2, p3, p4)                                    \
    if (!(p1) || !(p2) || !(p3) || !(p4)) {                                    \
        errno = EZI_ERR_NULL_ARGUMENTS;                                        \
        return -1;                                                             \
    }

#define CHECK_NULL_PARAMS_5(p1, p2, p3, p4, p5)                                \
    if (!(p1) || !(p2) || !(p3) || !(p4) || !(p5)) {                           \
        errno = EZI_ERR_NULL_ARGUMENTS;                                        \
        return -1;                                                             \
    }

#endif
