#include <ctype.h>
#include "utils.h"
#include "custom_assert.h"

reading_state_t read_file(const char *filename, parsed_text_t *text) {
    C_ASSERT(text     != NULL, UNKNOWN_READING_ERROR);
    C_ASSERT(filename != NULL, UNKNOWN_READING_ERROR);

    FILE *input = fopen(filename, "r");
    if(input == NULL)
        return READING_NO_SUCH_FILE;

    size_t size = file_size(input);
    text->input = (char *)calloc(size + sizeof(char), sizeof(char));
    if(text->input == NULL) {
        fclose(input);
        return READING_ALLOCATION_ERROR;
    }

    fread(text->input, sizeof(char), size, input);
    fclose(input);
    return READING_SUCCESS;
}

size_t file_size(FILE *file) {
    C_ASSERT(file != NULL, 0);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

parsing_state_t parse_strings(parsed_text_t *text) {
    C_ASSERT(text != NULL, UNKNOWN_PARSING_ERROR);

    if(text->input == NULL)
        return PARSING_BEFORE_READING_INPUT;

    for(size_t index = 0; text->input[index] != '\0'; index++)
        if(text->input[index] == '\n')
            text->strings_number++;

    text->lines = (char **)calloc(text->strings_number + 1, sizeof(char *));
    if(text->lines == NULL)
        return PARSING_ALLOCATION_ERROR;

    text->lines[0] = text->input;
    for(size_t index = 0, line = 1; text->input[index] != '\0'; index++) {
        if(text->input[index] == '\n') {
            C_ASSERT(line < text->strings_number + 1, UNKNOWN_PARSING_ERROR);
            text->input[index] = '\0';
            text->lines[line++] = text->input + index + 1;
        }
        if(text->input[index] == '\r')
            text->input[index] = '\0';

    }
    return PARSING_SUCCESS;
}

writing_state_t write_file(const char *filename, parsed_text_t *text) {
    C_ASSERT(filename != NULL, UNKNOWN_WRITING_ERROR);
    C_ASSERT(text     != NULL, UNKNOWN_WRITING_ERROR);

    FILE *output = fopen(filename, "w");
    if(output == NULL)
        return WRITING_OPEN_FILE_ERROR;

    for(size_t line = 0; line < text->strings_number; line++) {
        if(text->lines[line][0] != '\0') {
            if(fputs(text->lines[line], output) == EOF || fputc('\n', output) == EOF) {
                fclose(output);
                return WRITING_APPENDING_ERROR;
            }
        }
    }
    return WRITING_SUCCESS;
}

void free_text(parsed_text_t *text) {
    C_ASSERT(text != NULL, );

    free(text->input);
    text->input = NULL;
    free(text->lines);
    text->lines = NULL;
}

size_t rand_index(size_t size) {
    C_ASSERT(size != 0, 0);

    static size_t state = 0;
    state = 214013 * state + 2531011;
    return state % size;
}
