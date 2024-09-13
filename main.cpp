#include <stdlib.h>
#include <strings.h>
#include "colors.h"
#include "onegin.h"

static exit_code_t run_program(text_t *text);

int main(const int argc, const char *argv[]) {
    text_t text = {};
    switch(parse_input(&text, argc, argv)) {
        case PARSING_INPUT_ERROR:
            return EXIT_FAILURE;
        case PARSING_INPUT_SUCCESS:
            break;
        case PARSING_INPUT_EXIT:
            return EXIT_SUCCESS;
        default:
            return EXIT_FAILURE;
    }

    if(run_program(&text) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

exit_code_t run_program(text_t *text) {
    if(      try_read_file(text) != EXIT_CODE_SUCCESS) {
        free_text(text);
        return EXIT_CODE_FAILURE;
    }
    if(     try_parse_text(text) != EXIT_CODE_SUCCESS) {
        free_text(text);
        return EXIT_CODE_FAILURE;
    }
    if(try_sort_alphabetic(text) != EXIT_CODE_SUCCESS) {
        free_text(text);
        return EXIT_CODE_FAILURE;
    }
    if(     try_sort_rhyme(text) != EXIT_CODE_SUCCESS) {
        free_text(text);
        return EXIT_CODE_FAILURE;
    }
    if( try_print_original(text) != EXIT_CODE_SUCCESS) {
        free_text(text);
        return EXIT_CODE_FAILURE;
    }

    free_text(text);
    return EXIT_CODE_SUCCESS;
}
