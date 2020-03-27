#ifndef EZI_DATA_SOFTWARE_H
#define EZI_DATA_SOFTWARE_H

#include "ds/slist.h"

#include <stdint.h>
#include <stdio.h>

#define EZI_SOFTWARE_MAX_NAME_LEN 0x20
#define EZI_SOFTWARE_MAX_DESC_LEN 0x40
#define EZI_SOFTWARE_MAX_URL_LEN  0x60

struct ezi_software
{
    char name[EZI_SOFTWARE_MAX_NAME_LEN];
    char description[EZI_SOFTWARE_MAX_DESC_LEN];
    char url[EZI_SOFTWARE_MAX_URL_LEN];

    struct ezi_slist dependencies;
};

/*!
 * \brief Reads a software information from a file pointed to by \see fp and
 *        store the loaded data into the object pointed to by \see sw
 *
 * \param [out] sw  A pointer to the software object into which to load the data
 * \param [in]  fp  A pointer to the file from which to load the data
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_read(struct ezi_software *sw, FILE *fp);

/*!
 * \brief Writes a software information whcih is store in an object pointed to
 *        by \see sw into a file pointed to by \fp
 *
 * \param [in]  sw  A pointer to the software object which to write its data
 * \param [out] fp  A pointer to the file from into which to write the data
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_write(const struct ezi_software *sw, FILE *fp);

/*!
 * \brief Decodes a software object pointed to by \see sw from a byte
 *        buffer pointed to by \see buf of \see len length
 *
 * \param [in]     sw   A pointer to the software object which to be decoded
 * \param [in]     buf  A pointer to input buffer
 * \param [in,out] len  A pointer to the length of the input buffer
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_decode(struct ezi_software *sw, const void *buf, size_t *len);

/*!
 * \brief Encodes a software object pointed to by \see sw into a byte buffer
 *        pointed to by \see buf of \see len length
 *
 * \param [in]     sw   A pointer to the software object which to be encoded
 * \param [out]    buf  A pointer to output buffer
 * \param [in,out] len  A pointer to the length of the output buffer
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_encode(const struct ezi_software *sw, void *buf, size_t *len);

/*!
 * \brief Frees the resources used by the software object pointed to by \see sw
 *
 * \param [in] sw  A pointer to the software object to be freed
 */
void
free_ezi_software(struct ezi_software *sw);

#endif /* EZI_DATA_SOFTWARE_H */
