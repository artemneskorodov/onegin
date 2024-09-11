#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

enum reading_state_t {
    UNKNOWN_READING_ERROR,
    READING_SUCCESS,
    READING_NO_SUCH_FILE,
    READING_ALLOCATION_ERROR,
};

enum parsing_state_t {
    UNKNOWN_PARSING_ERROR,
    PARSING_SUCCESS,
    PARSING_ALLOCATION_ERROR,
    PARSING_BEFORE_READING_INPUT
};

enum writing_state_t {
    UNKNOWN_WRITING_ERROR,
    WRITING_SUCCESS,
    WRITING_OPEN_FILE_ERROR,
    WRITING_APPENDING_ERROR,
};

struct parsed_text_t {
    char *input;
    char **lines;
    size_t strings_number;
};

reading_state_t read_file(const char *filename, parsed_text_t *text);
size_t file_size(FILE *file);
parsing_state_t parse_strings(parsed_text_t *text);
writing_state_t write_file(const char *filename, parsed_text_t *text);
void free_text(parsed_text_t *text);
size_t rand_index(size_t size);

#endif
