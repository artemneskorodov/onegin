#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "onegin.h"

enum reading_state_t {
    UNKNOWN_READING_ERROR,
    READING_SUCCESS,
    READING_NO_SUCH_FILE,
    READING_ALLOCATION_ERROR
};

enum parsing_state_t {
    UNKNOWN_PARSING_ERROR,
    PARSING_SUCCESS,
    PARSING_ALLOCATION_ERROR,
};

enum writing_state_t {
    UNKNOWN_WRITING_ERROR,
    WRITING_SUCCESS,
    WRITING_OPEN_FILE_ERROR,
    WRITING_APPENDING_ERROR,
};

reading_state_t read_file(onegin_text_t *text);
parsing_state_t parse_lines(onegin_text_t *text);
writing_state_t write_file(const char *filename, onegin_text_t *text);
size_t rand_index(size_t size);
bool is_line_end(char symbol);

#endif
