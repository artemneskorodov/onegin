#include <stdarg.h>
#include <stdio.h>
#include "colors.h"
#include "custom_assert.h"

static const char *red_code    = "31";
static const char *green_code  = "32";
static const char *yellow_code = "33";
static const char *blue_code   = "34";
static const char *purple_code = "35";
static const char *cyan_code   = "36";
static const char *white_code  = "37";

static const char *black_background  = "40";
static const char *red_background    = "41";
static const char *green_background  = "42";
static const char *yellow_background = "43";
static const char *blue_background   = "44";
static const char *purple_background = "45";
static const char *cyan_background   = "46";
static const char *white_background  = "47";

static const char *bold = "1";

static const char *color_code_start = "\033[";

static void reset_color(void);
static void print_color_code(color_t color, bool is_bold, background_t background);
static const char *background_code(background_t background);
static const char *color_code(color_t color);

void color_printf(color_t color, bool is_bold, background_t background, const char *string, ...) {
    C_ASSERT(string != NULL, );

    print_color_code(color, is_bold, background);

    va_list args;
    va_start(args, string);
    vprintf(string, args);

    reset_color();
    va_end(args);
}

void print_color_code(color_t color, bool is_bold, background_t background) {
    printf("%s", color_code_start);

    //boldness
    if(is_bold == true) {
        printf("%s", bold);
        if(color != DEFAULT_TEXT || background != DEFAULT_BACKGROUND)
            printf(";");

        else {
            printf("m");
            return ;
        }
    }

    //color
    if(color != DEFAULT_TEXT) {
        printf("%s", color_code(color));
        if(background != DEFAULT_BACKGROUND)
            printf(";");
        else {
            printf("m");
            return ;
        }
    }

    //background
    if(background != DEFAULT_BACKGROUND) {
        printf("%sm", background_code(background));
        return ;
    }

    reset_color();
}

void reset_color(void){
    printf("%s0m", color_code_start);
}

const char *color_code(color_t color) {
    switch(color) {
        case RED_TEXT:
            return red_code;
        case GREEN_TEXT:
            return green_code;
        case YELLOW_TEXT:
            return yellow_code;
        case BLUE_TEXT:
            return blue_code;
        case PURPLE_TEXT:
            return purple_code;
        case CYAN_TEXT:
            return cyan_code;
        case WHITE_TEXT:
            return white_code;
        case DEFAULT_TEXT:
            return NULL;
        default:
            return NULL;
    }
}

const char *background_code(background_t background) {
    switch(background) {
    case BLACK_BACKGROUND:
        return black_background;
    case RED_BACKGROUND:
        return red_background;
    case GREEN_BACKGROUND:
        return green_background;
    case YELLOW_BACKGROUND:
        return yellow_background;
    case BLUE_BACKGROUND:
        return blue_background;
    case PURPLE_BACKGROUND:
        return purple_background;
    case CYAN_BACKGROUND:
        return cyan_background;
    case WHITE_BACKGROUND:
        return white_background;
    case DEFAULT_BACKGROUND:
        return NULL;
    default:
        return NULL;
    }
}
