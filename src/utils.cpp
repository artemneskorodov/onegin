#include <ctype.h>
#include "utils.h"
#include "custom_assert.h"

static size_t file_size(FILE *file);
static int file_put_line(FILE *file, char *string);

reading_state_t read_file(onegin_text_t *text) {
    C_ASSERT(text           != NULL, UNKNOWN_READING_ERROR);
    C_ASSERT(text->filename != NULL, UNKNOWN_READING_ERROR);

    FILE *input = fopen(text->filename, "rb");
    if(input == NULL)
        return READING_NO_SUCH_FILE;

    size_t size = file_size(input);
    text->input_text = (char *)calloc(size + sizeof(char), sizeof(char));
    if(text->input_text == NULL) {
        fclose(input);
        return READING_ALLOCATION_ERROR;
    }

    if(fread(text->input_text, sizeof(char), size, input) != size) {
        fclose(input);
        free(text->input_text);
        return UNKNOWN_READING_ERROR;
    }
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

parsing_state_t parse_lines(onegin_text_t *text) {
    C_ASSERT(text != NULL, UNKNOWN_PARSING_ERROR);

    if(text->input_text == NULL)
        return PARSING_BEFORE_READING_INPUT;

    for(const char *pointer = text->input_text; *pointer != '\0'; pointer++)
        if(*pointer == '\n')
            text->lines_number++;

    text->lines = (char **)calloc(text->lines_number + 1, sizeof(char *));
    if(text->lines == NULL)
        return PARSING_ALLOCATION_ERROR;

    text->lines[0] = text->input_text;
    size_t line = 1;
    for(char *pointer = text->input_text; *pointer != '\0'; pointer++) {
        if(*pointer == '\n') {
            C_ASSERT(line < text->lines_number + 1, UNKNOWN_PARSING_ERROR);
            text->lines[line++] = pointer + 1;
        }
    }
    return PARSING_SUCCESS;
}

writing_state_t write_file(const char *filename, char **lines, size_t lines_number) {
    C_ASSERT(filename != NULL, UNKNOWN_WRITING_ERROR);
    C_ASSERT(lines     != NULL, UNKNOWN_WRITING_ERROR);

    FILE *output = fopen(filename, "w");
    if(output == NULL)
        return WRITING_OPEN_FILE_ERROR;

    for(size_t line = 0; line < lines_number; line++) {
        if(lines[line][0] != '\0' && lines[line][0] != '\r' && lines[line][0] != '\n') {
            if(file_put_line(output, lines[line]) == EOF) {
                fclose(output);
                return WRITING_APPENDING_ERROR;
            }
        }
    }
    return WRITING_SUCCESS;
}

size_t rand_index(size_t size) {
    C_ASSERT(size != 0, 0);

    static size_t state = 0;
    state = 214013 * state + 2531011;
    return state % size;
}

int file_put_line(FILE *file, char *string) {
    C_ASSERT(file   != NULL, EOF);
    C_ASSERT(string != NULL, EOF);

    char last_symbol = 0;

    char *pointer = string;
    while(*pointer != '\r' && *pointer != '\n' && *pointer != '\0') {
        pointer++;
        last_symbol = *pointer;
    }
    *pointer = '\0';
    int fputs_result = fputs(string, file);
    if(fputs_result == EOF)
        return EOF;
    if(fputc('\n', file) == EOF)
        return EOF;
    *pointer = last_symbol;
    return fputs_result;
}
