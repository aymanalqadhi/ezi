#ifndef EZI_UTIL_MEMORY_H
#define EZI_UTIL_MEMORY_H

#include <stddef.h>

/*!
 * \brief Duplicates a buffer
 *
 * \param [in] buf  The buffer to duplicate
 * \param [in] len  The buffer size
 * \return A malloced duplicate buffer, or NULL on failure
 */
void *
ezi_memdup(const void *buf, size_t len);

#endif /* EZI_UTIL_MEMORY_H */
