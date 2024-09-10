#include <strings.h>
#include "utils.h"
#include "colors.h"
#include "handle_flags.h"

int main(const int argc, const char *argv[]) {
    program_mode_t mode = {};
    parsed_text_t text = {};

    if(parse_flags(argc, argv, &mode) != PARSING_FLAGS_SUCCESS) {
        free_text(&text);
        return EXIT_FAILURE;
    }

    if(try_read_file(&text, &mode) != EXIT_CODE_SUCCESS) {
        free_text(&text);
        return EXIT_FAILURE;
    }

    if(try_parse_text(&text) != EXIT_CODE_SUCCESS) {
        free_text(&text);
        return EXIT_FAILURE;
    }

    if(try_sort_text(&text, &mode) != EXIT_CODE_SUCCESS) {
        free_text(&text);
        return EXIT_FAILURE;
    }

    if(try_write_output(&text, &mode) != EXIT_CODE_SUCCESS) {
        free_text(&text);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
