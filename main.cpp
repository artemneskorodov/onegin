#include <stdlib.h>
#include <strings.h>
#include "colors.h"
#include "onegin.h"
#include "custom_assert.h"

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
    C_ASSERT(text != NULL, return EXIT_CODE_FAILURE);

    typedef exit_code_t (*trier_t)(text_t *);

    const trier_t triers[] = {try_read_file      ,
                              try_parse_text     ,
                              try_sort_alphabetic,
                              try_sort_rhyme     ,
                              try_print_original };

    for(size_t trier = 0; trier < sizeof(triers) / sizeof(triers[0]); trier++)
        if(triers[trier](text) != EXIT_CODE_SUCCESS) {
            free_text(text);
            return EXIT_CODE_FAILURE;
        }

    free_text(text);
    return EXIT_CODE_SUCCESS;
}
