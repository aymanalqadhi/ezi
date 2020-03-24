#ifndef EZI_UTIL_ENDIAN_H
#define EZI_UTIL_ENDIAN_H

#include <stdint.h>

/*!
 * \brief Checks whether the current running CPU is using big-endian byte
 *        representation or not
 *
 *  \return 1 if the current CPU is using big-endian, 0 if not
 */
int
is_big_endian(void);

/*!
 * \brief Swaps the endianness of a 16-bit unsigned integer
 *
 * \param [in] num  The integer to swap
 *
 * \return The endianness-swapped value
 */
uint16_t
ezi_swap_endianness_16(uint16_t num);

/*!
 * \brief Swaps the endianness of a 32-bit unsigned integer
 *
 * \param [in] num  The integer to swap
 *
 * \return The endianness-swapped value
 */
uint32_t
ezi_swap_endianness_32(uint32_t num);

/*!
 * \brief Swaps the endianness of a 64-bit unsigned integer
 *
 * \param [in] num  The integer to swap
 *
 * \return The endianness-swapped value
 */
uint64_t
ezi_swap_endianness_64(uint64_t num);

/*!
 * \brief Converts a 16-bit unsigned integer from host representation
 *        to big-endian representation
 *
 * \param [in] num  The integer to convert
 *
 * \return The converted value
 */
uint16_t
ezi_htobe16(uint16_t num);

/*!
 * \brief Converts a 32-bit unsigned integer from host representation
 *        to big-endian representation
 *
 * \param [in] num  The integer to convert
 *
 * \return The converted value
 */
uint32_t
ezi_htobe32(uint32_t num);

/*!
 * \brief Converts a 64-bit unsigned integer from host representation
 *        to big-endian representation
 *
 * \param [in] num  The integer to convert
 *
 * \return The converted value
 */
uint64_t
ezi_htobe64(uint64_t num);

/*!
 * \brief Converts a 16-bit unsigned integer from host big-endian
 *        to host representation
 *
 * \param [in] num  The integer to convert
 *
 * \return The converted value
 */
uint16_t
ezi_betoh16(uint16_t num);

/*!
 * \brief Converts a 32-bit unsigned integer from host big-endian
 *        to host representation
 *
 * \param [in] num  The integer to convert
 *
 * \return The converted value
 */
uint32_t
ezi_betoh32(uint32_t num);

/*!
 * \brief Converts a 64-bit unsigned integer from host big-endian
 *        to host representation
 *
 * \param [in] num  The integer to convert
 *
 * \return The converted value
 */
uint64_t
ezi_betoh64(uint64_t num);

#endif /* EZI_UTIL_ENDIAN_H */
