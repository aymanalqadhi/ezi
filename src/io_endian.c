#include "io/endian.h"

#include <stdint.h>

static const int endianness_check_num = 0xAABBCCDD;

inline int
is_big_endian(void)
{
    return *((uint8_t *)&endianness_check_num) == 0xAA;
}

inline uint16_t
ezi_swap_endianness_16(uint16_t num)
{
    num = ((num & 0x00FF) << 0x08) | ((num & 0xFF00) >> 0x08);
    return num;
}

inline uint32_t
ezi_swap_endianness_32(uint32_t num)
{
    num = ((num & 0x000000FF) << 0x18) | ((num & 0x0000FF00) << 0x08) |
          ((num & 0x00FF0000) >> 0x08) | ((num & 0xFF000000) >> 0x18);

    return num;
}

inline uint64_t
ezi_swap_endianness_64(uint64_t num)
{
    num = ((num & 0x00000000000000FF) << 0x38) |
          ((num & 0x000000000000FF00) << 0x28) |
          ((num & 0x0000000000FF0000) << 0x18) |
          ((num & 0x00000000FF000000) << 0x08) |
          ((num & 0x000000FF00000000) >> 0x08) |
          ((num & 0x0000FF0000000000) >> 0x18) |
          ((num & 0x00FF000000000000) >> 0x28) |
          ((num & 0xFF00000000000000) >> 0x38);

    return num;
}

inline uint16_t
ezi_htobe16(uint16_t num)
{
    return is_big_endian() ? num : ezi_swap_endianness_16(num);
}

inline uint32_t
ezi_htobe32(uint32_t num)
{
    return is_big_endian() ? num : ezi_swap_endianness_32(num);
}

inline uint64_t
ezi_htobe64(uint64_t num)
{
    return is_big_endian() ? num : ezi_swap_endianness_64(num);
}

inline uint16_t
ezi_betoh16(uint16_t num)
{
    return is_big_endian() ? num : ezi_swap_endianness_16(num);
}

inline uint32_t
ezi_betoh32(uint32_t num)
{
    return is_big_endian() ? num : ezi_swap_endianness_32(num);
}

inline uint64_t
ezi_betoh64(uint64_t num)
{
    return is_big_endian() ? num : ezi_swap_endianness_64(num);
}
