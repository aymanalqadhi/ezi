#ifndef EZI_UTIL_PATH_H
#define EZI_UTIL_PATH_H

#include <stddef.h>

/*!
 * \brief Gets the base filename with extension from a full path
 *
 * \param [in]  str     A pointer to the full path string
 * \param [in]  en      The length of the full path string
 * \param [out] out     A pointer to the output base filename string
 * \param [in]  outlen  The length of the output base filname string
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_base_filename(const char *str, size_t len, char *out, size_t outlen);

/*!
 * \brief Gets the base filename without extension from a full path
 *
 * \param [in]  str     A pointer to the full path string
 * \param [in]  en      The length of the full path string
 * \param [out] out     A pointer to the output base filename string
 * \param [in]  outlen  The length of the output base filname string
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_base_filename_no_ext(const char *str, size_t len, char *out, size_t outlen);

#endif
