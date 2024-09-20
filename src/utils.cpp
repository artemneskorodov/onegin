#include <ctype.h>
#include "utils.h"
#include "custom_assert.h"

enum writing_line_result_t {
    WRITING_LINE_SUCCESS,
    WRITING_LINE_ERROR  ,
};

static size_t file_size(FILE *file);
static writing_line_result_t file_put_line(FILE *file, line_t *line);
static exit_code_t add_line_end(char *text);

reading_state_t read_file(text_t *text) {
    C_ASSERT(text             != NULL, return UNKNOWN_READING_ERROR);
    C_ASSERT(text->filename   != NULL, return UNKNOWN_READING_ERROR);
    C_ASSERT(text->input_text == NULL, return UNKNOWN_READING_ERROR);

    FILE *input = fopen(text->filename, "rb");
    if(input == NULL)
        return READING_NO_SUCH_FILE;

    text->input_length = file_size(input);
    if(text->input_length == 0) {
        fclose(input);
        return UNKNOWN_READING_ERROR;
    }
    text->input_text = (char *)calloc(text->input_length + 3, sizeof(text->input_text[0]));
    if(text->input_text == NULL) {
        fclose(input);
        return READING_ALLOCATION_ERROR;
    }

    if(fread(text->input_text, 1, text->input_length, input) !=
       text->input_length) {
        fclose(input);
        free_text(text);
        return UNKNOWN_READING_ERROR;
    }

    if(add_line_end(text->input_text) != EXIT_CODE_SUCCESS) {
        fclose(input);
        free_text(text);
        return UNKNOWN_READING_ERROR;
    }

    fclose(input);
    return READING_SUCCESS;
}

size_t file_size(FILE *file) {
    C_ASSERT(file != NULL, return 0);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}

parsing_state_t parse_lines(text_t *text) {
    C_ASSERT(text             != NULL, return UNKNOWN_PARSING_ERROR);
    C_ASSERT(text->input_text != NULL, return UNKNOWN_PARSING_ERROR);
    C_ASSERT(text->lines      == NULL, return UNKNOWN_PARSING_ERROR);

    for(char *buffer_pointer = text->input_text; *buffer_pointer != '\0'; buffer_pointer++)
        if(*buffer_pointer == '\n')
            text->lines_number++;

    text->lines = (line_t *)calloc(text->lines_number, sizeof(text->lines[0]));
    if(text->lines == NULL)
        return PARSING_ALLOCATION_ERROR;

    text->lines[0].start = text->input_text;
    size_t line = 0;
    for(char *buffer_pointer = text->input_text; *buffer_pointer != '\0'; buffer_pointer++) {
        if(!is_line_end(*buffer_pointer))
            text->lines[line].length++;
        if(*buffer_pointer == '\n') {
            C_ASSERT(line < text->lines_number + 1, return UNKNOWN_PARSING_ERROR);
            text->lines[++line].start = buffer_pointer + 1;
        }
    }
    return PARSING_SUCCESS;
}

writing_state_t write_file(const char *filename, text_t *text) {
    C_ASSERT(filename    != NULL, return UNKNOWN_WRITING_ERROR);
    C_ASSERT(text        != NULL, return UNKNOWN_WRITING_ERROR);
    C_ASSERT(text->lines != NULL, return UNKNOWN_WRITING_ERROR);

    FILE *output = fopen(filename, "wb");
    if(output == NULL)
        return WRITING_OPEN_FILE_ERROR;

    for(size_t line = 0; line < text->lines_number; line++) {
        if(file_put_line(output, text->lines + line) != WRITING_LINE_SUCCESS) {
            fclose(output);
            return WRITING_APPENDING_ERROR;
        }
    }
    fclose(output);
    return WRITING_SUCCESS;
}

size_t rand_index(size_t size) {
    C_ASSERT(size != 0, return 0);

    static size_t state = 0;
    state = 5635744375709631613 * state + 233674107310555513;
    return state % size;
}

writing_line_result_t file_put_line(FILE *file, line_t *line) {
    C_ASSERT(file        != NULL, return WRITING_LINE_ERROR);
    C_ASSERT(line        != NULL, return WRITING_LINE_ERROR);
    C_ASSERT(line->start != NULL, return WRITING_LINE_ERROR);

    if(fwrite(line->start, sizeof(char), line->length, file) != line->length)
        return WRITING_LINE_ERROR;

    if(fputc('\n', file) == EOF)
        return WRITING_LINE_ERROR;

    return WRITING_LINE_SUCCESS;
}

bool is_line_end(char symbol) {
    if(symbol == '\r' || symbol == '\n' || symbol == '\0')
        return true;

    return false;
}

exit_code_t add_line_end(char *text) {
    C_ASSERT(text != NULL, return EXIT_CODE_FAILURE);

    while(*text != '\0')
        text++;

    if(!is_line_end(*(text - 1))) {
        *(text++) = '\r';
        *(text++) = '\n';
        *(text  ) = '\0';
    }
    return EXIT_CODE_SUCCESS;
}
