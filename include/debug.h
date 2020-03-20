#ifndef EZI_DEBUG_H
#define EZI_DEBUG_H

#include <assert.h>

/* An extended assert macro with message support */
#define massert(exp, msg) assert(exp && msg)

#endif /* EZI_DEBUG_H */
