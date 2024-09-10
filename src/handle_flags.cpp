#include <strings.h>
#include "handle_flags.h"
#include "custom_assert.h"
#include "colors.h"
#include "sort.h"

static const program_mode_t DEFAULT_MODE =
    {.input_file_name = "Onegin.txt", .output_file_name = "output.txt", .compare_function = string_compare_alphabetic};

parsing_flags_state_t parse_flags(const int argc, const char *argv[], program_mode_t *mode) {
    C_ASSERT(argv != NULL, PARSING_FLAGS_ERROR);
    C_ASSERT(mode != NULL, PARSING_FLAGS_ERROR);

    for(int index = 1; index < argc; index++) {
        if(strcmp(argv[index], "--input") == 0 ||
           strcmp(argv[index], "-i"     ) == 0) {

            if(index + 1 >= argc) {
                color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Expected name of file after key word '%s'.\n", argv[index]);
                return PARSING_FLAGS_ERROR;
            }
            mode->input_file_name = argv[++index];
        }
        else if(strcmp(argv[index], "--output") == 0 ||
                strcmp(argv[index], "-o"      ) == 0) {

            if(index + 1 >= argc) {
                color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Expected name of file after key word '%s'.\n", argv[index]);
                return PARSING_FLAGS_ERROR;
            }
            mode->output_file_name = argv[++index];
        }
        else if(strcmp(argv[index], "--rhyme") == 0 ||
                strcmp(argv[index], "-r"     ) == 0) {

            mode->compare_function = string_compare_rhyme;
        }
        else if(strcmp(argv[index], "--alpha") == 0 ||
                strcmp(argv[index], "-a"     ) == 0) {

            mode->compare_function = string_compare_alphabetic;
        }
        else {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unexpected parameter: %s.\n", argv[index]);
            return PARSING_FLAGS_ERROR;
        }
    }

    if(mode->compare_function == NULL)
        mode->compare_function = DEFAULT_MODE.compare_function;
    if(mode->input_file_name == NULL)
        mode->input_file_name = DEFAULT_MODE.input_file_name;
    if(mode->output_file_name == NULL)
        mode->output_file_name = DEFAULT_MODE.output_file_name;
    return PARSING_FLAGS_SUCCESS;
}

exit_code_t try_read_file(parsed_text_t *text, program_mode_t *mode) {
    C_ASSERT(text                  != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(mode                  != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(mode->input_file_name != NULL, EXIT_CODE_FAILURE);

    switch(read_file(mode->input_file_name, text)) {
        case UNKNOWN_READING_ERROR: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unknown reading error (or assert).\n");
            return EXIT_CODE_FAILURE;
        }
        case READING_SUCCESS: {
            color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND, "Successfully read from file.\n");
            return EXIT_CODE_SUCCESS;
        }
        case READING_NO_SUCH_FILE: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "There is no file \"%s\".\n", mode->input_file_name);
            return EXIT_CODE_FAILURE;
        }
        case READING_ALLOCATION_ERROR: {
            free_text(text);
            color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND, "Error while allocating memmory.\n");
            return EXIT_CODE_FAILURE;
        }
        default: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unexpected read_file(...) result.\n");
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t try_parse_text(parsed_text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    switch(parse_strings(text)) {
        case UNKNOWN_PARSING_ERROR: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unexpected error while parsing text into strings.\n");
            return EXIT_CODE_FAILURE;
        }
        case PARSING_SUCCESS: {
            color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND, "Successfully parsed text into strings.\n");
            return EXIT_CODE_SUCCESS;
        }
        case PARSING_ALLOCATION_ERROR: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Error while allocating memmory.\n");
            return EXIT_CODE_FAILURE;
        }
        case PARSING_BEFORE_READING_INPUT: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Parsing text into lines before reading lines.\n");
            return EXIT_CODE_FAILURE;
        }
        default: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unexpected return value of parse_strings(...).\n");
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t try_sort_text(parsed_text_t *text, program_mode_t *mode) {
    C_ASSERT(text                   != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(mode                   != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(mode->compare_function != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(text->input            != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(text->lines            != NULL, EXIT_CODE_FAILURE);

    if(sort_array(text->lines, sizeof(char *), text->strings_number, mode->compare_function) != SORTING_SUCCESS)
        return EXIT_CODE_FAILURE;

    return EXIT_CODE_SUCCESS;
}

exit_code_t try_write_output(parsed_text_t *text, program_mode_t *mode) {
    C_ASSERT(text                   != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(mode                   != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(mode->output_file_name != NULL, EXIT_CODE_FAILURE);

    switch(write_file(mode->output_file_name, text)) {
        case UNKNOWN_WRITING_ERROR: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Caught unsupported error, while running write_file(...).\n");
            return EXIT_CODE_FAILURE;
        }
        case WRITING_SUCCESS: {
            free_text(text);
            color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND, "Successfully wrote sorted text to file '%s'.\n", mode->output_file_name);
            return EXIT_CODE_SUCCESS;
        }
        case WRITING_OPEN_FILE_ERROR: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unable to write in '%s'.\n", mode->output_file_name);
            return EXIT_CODE_FAILURE;
        }
        case WRITING_APPENDING_ERROR:{
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Error while writing text to file.\n");
            return EXIT_CODE_FAILURE;
        }
        default: {
            free_text(text);
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Unexpected return value for write_file(...).\n");
            return EXIT_CODE_SUCCESS;
        }
    }
}
