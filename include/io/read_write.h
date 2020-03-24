#ifndef EZI_IO_READ_WRITE_H
#define EZI_IO_READ_WRITE_H

#include <stdint.h>
#include <stdio.h>

/*!
 * \brief Writes a \see len bytes memory buffer into a file pointed by \see fp
 *
 * \param [in]  buf  A pointer to the memory buffer to be written
 * \param [in]  len  The number of bytes to be written
 * \param [out] fp   A pointer to the file into which to write
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_write(const void *buf, size_t len, FILE *fp);

/*!
 * \brief Writes an unsigned 16-bit integer to a file pointed by \see fp
 *
 * \param [in]  num  The integer value to be written
 * \param [out] fp   A pointer to the file into which to write
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_write_uint16(uint16_t num, FILE *fp);

/*!
 * \brief Writes an unsigned 32-bit integer to a file pointed to by \see fp
 *
 * \param [in]  num  The integer value to be written
 * \param [out] fp   A pointer to the file into which to write
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_write_uint32(uint32_t num, FILE *fp);

/*!
 * \brief Writes an unsigned 64-bit integer to a file pointed to by \see fp
 *
 * \param [in]  num  The integer value to be written
 * \param [out] fp   A pointer to the file into which to write
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_write_uint64(uint64_t num, FILE *fp);

/*!
 * \brief Writes a NULL-Terminates string into a file pointed to by \see fp
 *
 * \param [in]  str  A pointer to the string to be written
 * \param [out] fp   A pointer to the file into which to write
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_write_string(const char *str, FILE *fp);

/*!
 * \brief Reads \see len bytes and store them in the buffer pointed to by
 *        \see buf from the file pointed to by \see fp
 *
 * \param [out]  buf  A pointer to the buffer which to read into
 * \param [in]   len  The number of bytes to be read
 * \param [in]  fp    A pointer to the file to read from
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_read(void *buf, size_t len, FILE *fp);

/*!
 * \brief Reads an unsigned 16-bit integer from a file pointed to by \see fp
 *
 * \param [out] nump  A pointer to the output number
 * \param [in]  fp    A pointer to the file to read from
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_read_uint16(uint16_t *nump, FILE *fp);

/*!
 * \brief Reads an unsigned 32-bit integer from a file pointed to by \see fp
 *
 * \param [out] nump  A pointer to the output number
 * \param [in]  fp    A pointer to the file to read from
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_read_uint32(uint32_t *nump, FILE *fp);

/*!
 * \brief Reads an unsigned 64-bit integer from a file pointed to by \see fp
 *
 * \param [out] nump  A pointer to the output number
 * \param [in]  fp    A pointer to the file to read from
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_read_uint64(uint64_t *nump, FILE *fp);

/*!
 * \brief Reads a string of \see *len maximum length, and store it in the buffer
 *        pointed to by \see buf, with the length stored in *len
 *
 * \param [out]    buf  A pointer to the buffer which to read into
 * \param [in,out] len  A pointer to length of the buffer, this read string
 *                      length will be stored in this variable
 * \param [in]  fp      A pointer to the file to read from
 *
 * \return 0 for success, -1 otherwise with errno set
 */
int
ezi_fs_read_string(char *buf, size_t *len, FILE *fp);

#endif /* EZI_IO_READ_WRITE_H */
