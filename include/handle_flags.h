#ifndef HANDLE_FLAGS_H
#define HANDLE_FLAGS_H

#include "utils.h"

struct program_mode_t {
    const char *input_file_name;
    const char *output_file_name;
    int (*compare_function)(const void *, const void *);
};

enum parsing_flags_state_t {
    PARSING_FLAGS_ERROR,
    PARSING_FLAGS_SUCCESS
};

enum exit_code_t {
    EXIT_CODE_SUCCESS,
    EXIT_CODE_FAILURE
};

parsing_flags_state_t parse_flags(const int argc, const char *argv[], program_mode_t *mode);
exit_code_t try_read_file(parsed_text_t *text, program_mode_t *mode);
exit_code_t try_parse_text(parsed_text_t *text, program_mode_t *);
exit_code_t try_sort_text(parsed_text_t *text, program_mode_t *mode);
exit_code_t try_write_output(parsed_text_t *text, program_mode_t *mode);

#endif
