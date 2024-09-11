#include <strings.h>
#include "utils.h"
#include "colors.h"
#include "handle_flags.h"

static exit_code_t (*const try_functions[])(parsed_text_t *text, program_mode_t *mode) = {
    try_read_file   ,
    try_parse_text  ,
    try_sort_text   ,
    try_write_output};

int main(const int argc, const char *argv[]) {
    program_mode_t mode = {};
    if(parse_flags(argc, argv, &mode) != PARSING_FLAGS_SUCCESS)
        return EXIT_FAILURE;

    parsed_text_t text = {};
    for(size_t trier = 0; trier < sizeof(try_functions) / sizeof(try_functions[0]); trier++)
        if(try_functions[trier](&text, &mode) != EXIT_CODE_SUCCESS) {
            free_text(&text);
            return EXIT_FAILURE;
        }

    free_text(&text);
    return EXIT_SUCCESS;
}
