#include "utils.h"
#include "custom_assert.h"

state_code_t read_file(const char *filename, char **output) {
    C_ASSERT(filename != NULL, READING_ERROR);
    C_ASSERT(output   != NULL, READING_ERROR);

    FILE *input = fopen(filename, "r");
    if(input == NULL)
        return READING_NO_SUCH_FILE;

    size_t size = file_size(input);
    *output = (char *)calloc(size + sizeof(char), sizeof(char));
    if(*output == NULL) {
        fclose(input);
        return READING_ALLOCATION_ERROR;
    }

    fread(*output, sizeof(char), size, input);
    fclose(input);
    return READING_SUCCESS;
}

size_t file_size(FILE *file) {
    C_ASSERT(file != NULL);

    fseek(file, 0, 2);
    size_t size = ftell(file);
    fseek(file, 0, 0);
    return size;
}

parsing_state_t parse_strings(char *input, parsed_text_t *output) {
    C_ASSERT(input  != NULL, PARSING_ERROR);
    C_ASSERT(output != NULL, PARSING_ERROR);

    for(size_t index = 0; input[index] != '\0'; index++)
        if(input[index] == '\n')
            output->strings_number++;

    output->lines = (char **)calloc(output->strings_number, sizeof(char *));
    if(output->lines == NULL)
        return PARSING_ALLOCATION_ERROR;

    output->lines[0] = input;
    for(size_t index = 0, line = 1; input[index] != '\0'; index++)
        if(input[index] == '\n') {
            C_ASSERT(line < output->strings_number, PARSING_ERROR);

            output->lines[line++] = input[index + 1];
            input[index] = '\0';
        }
    return PARSING_SUCCESS;
}
