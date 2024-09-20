#ifndef COLORS_H
#define COLORS_H

enum color_t {
    RED_TEXT    ,
    GREEN_TEXT  ,
    YELLOW_TEXT ,
    BLUE_TEXT   ,
    MAGENTA_TEXT,
    CYAN_TEXT   ,
    WHITE_TEXT  ,
    DEFAULT_TEXT,
};

enum background_t {
    BLACK_BACKGROUND  ,
    RED_BACKGROUND    ,
    GREEN_BACKGROUND  ,
    YELLOW_BACKGROUND ,
    BLUE_BACKGROUND   ,
    PURPLE_BACKGROUND ,
    CYAN_BACKGROUND   ,
    WHITE_BACKGROUND  ,
    DEFAULT_BACKGROUND,
};

int color_printf(color_t      color,
                 bool         is_bold,
                 background_t background,
                 const char * format, ...);

#endif
