#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>

enum exit_code_t {
    EXIT_CODE_SUCCESS,
    EXIT_CODE_FAILURE
};

enum parsing_input_exit_code_t {
    PARSING_INPUT_ERROR  ,
    PARSING_INPUT_SUCCESS,
    PARSING_INPUT_EXIT   ,
};

struct text_t {
    const char *filename;
    char *      input_text;
    char **     lines;
    size_t      lines_number;
};

exit_code_t try_read_file      (text_t *text);
exit_code_t try_parse_text     (text_t *text);
exit_code_t try_sort_alphabetic(text_t *text);
exit_code_t try_sort_rhyme     (text_t *text);
exit_code_t try_print_original (text_t *text);
parsing_input_exit_code_t parse_input(text_t *text, const int argc, const char *argv[]);
void free_text(text_t *text);

#endif
