#ifndef EZDB_LOG_COLORS_H
#define EZDB_LOG_COLORS_H

#include <stdint.h>

enum term_colors
{
    COLOR_RESET = 0x30,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN
};

#define COLOR_FORMAT_STR "\e[%x;%xm"
#define COLOR_RESET_STR  "\e[0m"

#define COLOR_FLAG_BOLD 0x1

#define COMBINE_COLOR(color, flags) (uint8_t)(color | (flags << 8))
#define GET_COLOR_COLOR(color)      (color & 0xFF)
#define GET_COLOR_FLAGS(color)      ((color >> 8) & 0xFF)

#endif /* EZDB_LOG_COLORS_H */
