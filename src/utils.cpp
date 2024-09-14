#include <ctype.h>
#include "utils.h"
#include "custom_assert.h"

static size_t file_size(FILE *file);
static int file_put_line(FILE *file, char *string);
static void add_line_end(char *text);

reading_state_t read_file(text_t *text) {
    C_ASSERT(text             != NULL, UNKNOWN_READING_ERROR);
    C_ASSERT(text->filename   != NULL, UNKNOWN_READING_ERROR);
    C_ASSERT(text->input_text == NULL, UNKNOWN_READING_ERROR);

    FILE *input = fopen(text->filename, "rb");
    if(input == NULL)
        return READING_NO_SUCH_FILE;

    text->input_length = file_size(input);
    if(text->input_length == 0) {
        fclose(input);
        return UNKNOWN_READING_ERROR;
    }
    text->input_text = (char *)calloc(text->input_length + 3, 1);
    if(text->input_text == NULL) {
        fclose(input);
        return READING_ALLOCATION_ERROR;
    }

    if(fread(text->input_text, 1, text->input_length, input) != text->input_length) {
        fclose(input);
        free(text->input_text);
        return UNKNOWN_READING_ERROR;
    }

    add_line_end(text->input_text);

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

parsing_state_t parse_lines(text_t *text) {
    C_ASSERT(text             != NULL, UNKNOWN_PARSING_ERROR);
    C_ASSERT(text->input_text != NULL, UNKNOWN_PARSING_ERROR);
    C_ASSERT(text->lines      == NULL, UNKNOWN_PARSING_ERROR);

    for(const char *buffer_pointer = text->input_text; *buffer_pointer != '\0'; buffer_pointer++)
        if(*buffer_pointer == '\n')
            text->lines_number++;

    text->lines = (char **)calloc(text->lines_number + 1, sizeof(char *));
    if(text->lines == NULL)
        return PARSING_ALLOCATION_ERROR;

    text->lines[0] = text->input_text;
    size_t line = 1;
    for(char *buffer_pointer = text->input_text; *buffer_pointer != '\0'; buffer_pointer++) {
        if(*buffer_pointer == '\n') {
            C_ASSERT(line < text->lines_number + 1, UNKNOWN_PARSING_ERROR);
            text->lines[line++] = buffer_pointer + 1;
        }
    }
    return PARSING_SUCCESS;
}

writing_state_t write_file(const char *filename, text_t *text) {
    C_ASSERT(filename != NULL, UNKNOWN_WRITING_ERROR);
    C_ASSERT(text     != NULL, UNKNOWN_WRITING_ERROR);

    FILE *output = fopen(filename, "wb");
    if(output == NULL)
        return WRITING_OPEN_FILE_ERROR;

    for(size_t line = 0; line < text->lines_number; line++) {
        if(file_put_line(output, text->lines[line]) == EOF) {
            fclose(output);
            return WRITING_APPENDING_ERROR;
        }
    }
    fclose(output);
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

    char *buffer_pointer = string;
    while(!is_line_end(*buffer_pointer)) {
        buffer_pointer++;
        last_symbol = *buffer_pointer;
    }
    *buffer_pointer = '\0';
    int fputs_result = fputs(string, file);
    if(fputs_result == EOF)
        return EOF;
    if(fputc('\n', file) == EOF)
        return EOF;
    *buffer_pointer = last_symbol;
    return fputs_result;
}

bool is_line_end(char symbol) {
    if(symbol == '\r' || symbol == '\n' || symbol == '\0')
        return true;

    return false;
}

void add_line_end(char *text) {
    C_ASSERT(text != NULL, );

    while(*text != '\0')
        text++;

    if(!is_line_end(*(text - 1))) {
        *(text++) = '\r';
        *(text++) = '\n';
        *(text  ) = '\0';
    }
}
