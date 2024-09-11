#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>

enum exit_code_t {
    EXIT_CODE_SUCCESS,
    EXIT_CODE_FAILURE
};

struct onegin_text_t {
    const char *filename;
    char *input_text;
    char **lines;
    size_t lines_number;
};

exit_code_t try_read_file(onegin_text_t *text);
exit_code_t try_parse_text(onegin_text_t *text);
exit_code_t try_sort_alphabetic(onegin_text_t *text);
exit_code_t try_sort_rhyme(onegin_text_t *text);
exit_code_t try_print_original(onegin_text_t *text);
void free_text(onegin_text_t *text);

#endif
