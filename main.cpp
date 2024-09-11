#include <stdlib.h>
#include "utils.h"
#include "colors.h"
#include "onegin.h"

static const char *DEFAULT_READ_FILE_NAME = "Onegin.txt";

int main(const int argc, const char *argv[]) {
    onegin_text_t text = {};
    text.filename = DEFAULT_READ_FILE_NAME;
    if(argc > 2) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unexpected parameter '%s'.\n", argv[2]);
        return EXIT_FAILURE;
    }
    if(argc == 2)
        text.filename = argv[1];

    if(try_read_file(&text) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;
    if(try_parse_text(&text) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;
    if(try_sort_alphabetic(&text) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;
    if(try_sort_rhyme(&text) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;
    if(try_print_original(&text) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;

    free_text(&text);
    return EXIT_SUCCESS;
}
