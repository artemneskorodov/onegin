#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

enum reading_state_t {
    READING_NO_SUCH_FILE,
    READING_ALLOCATION_ERROR,
    READING_ERROR,
    READING_SUCCESS
};

enum parsing_state_t {
    PARSING_ERROR,
    PARSING_ALLOCATION_ERROR,
    PARSING_SUCCESS
};

struct parsed_text_t {
    char **lines;
    size_t strings_number;
};

reading_state_t read_file(const char *filename, char **output);
size_t file_size(FILE *file);
parsing_state_t parse_strings(char *input, parsed_text_t *output);

#endif
