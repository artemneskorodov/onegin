#include <strings.h>
#include <stdbool.h>
#include "handle_flags.h"
#include "custom_assert.h"
#include "colors.h"
#include "sort.h"

struct flags_t {
    const char *long_name;
    const char *short_name;
    bool (*set_mode)(const char **params, program_mode_t *mode);
    int arguements_number;
};

static bool set_mode_input (const char **params, program_mode_t *mode);
static bool set_mode_output(const char **params, program_mode_t *mode);
static bool set_mode_rhyme (const char **params, program_mode_t *mode);
static bool set_mode_alpha (const char **params, program_mode_t *mode);
static void set_default_options(program_mode_t *mode);

static const flags_t supported_flags[] = {
    {.long_name = "--input" , .short_name = "-i", .set_mode = set_mode_input , .arguements_number = 1},
    {.long_name = "--output", .short_name = "-o", .set_mode = set_mode_output, .arguements_number = 1},
    {.long_name = "--rhyme" , .short_name = "-r", .set_mode = set_mode_rhyme , .arguements_number = 0},
    {.long_name = "--alpha" , .short_name = "-a", .set_mode = set_mode_alpha , .arguements_number = 0}};

static const program_mode_t DEFAULT_MODE =
    {.input_file_name = "Onegin.txt", .output_file_name = "output.txt", .compare_function = string_compare_alphabetic};

parsing_flags_state_t parse_flags(const int argc, const char *argv[], program_mode_t *mode) {
    C_ASSERT(argv != NULL, PARSING_FLAGS_ERROR);
    C_ASSERT(mode != NULL, PARSING_FLAGS_ERROR);

    for(int index = 1; index < argc; index++)
        for(size_t flag = 0; flag < sizeof(supported_flags) / sizeof(flags_t); flag++)
            if(strcmp(argv[index], supported_flags[flag].long_name ) == 0 ||
               strcmp(argv[index], supported_flags[flag].short_name) == 0) {

                if(supported_flags[flag].arguements_number + index >= argc) {
                    color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                                 "Flag '%s' is expected to have %d arguements after it.\n",
                                 argv[index], supported_flags[flag].arguements_number);
                    return PARSING_FLAGS_ERROR;
                }

                if(supported_flags[flag].set_mode(argv + index + 1, mode) == false) {
                    color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                                 "The field to flag '%s' is alredy set, don't use flags with same meaning twice",
                                 argv[index]);
                    return PARSING_FLAGS_ERROR;
                }

                index += supported_flags[flag].arguements_number;
                break;
            }
    set_default_options(mode);
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

    if(sort_array(text->lines, sizeof(char *), text->strings_number, mode->compare_function) != SORTING_SUCCESS) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND, "Error while sorting lines.\n");
        return EXIT_CODE_FAILURE;
    }

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

bool set_mode_input(const char **params, program_mode_t *mode) {
    C_ASSERT(params                != NULL, false);
    C_ASSERT(mode                  != NULL, false);

    if(mode->input_file_name != NULL)
        return false;

    mode->input_file_name = params[0];
    return true;
}

bool set_mode_output(const char **params, program_mode_t *mode) {
    C_ASSERT(params                 != NULL, false);
    C_ASSERT(mode                   != NULL, false);

    if(mode->output_file_name != NULL)
        return false;

    mode->output_file_name = params[0];
    return true;
}

bool set_mode_rhyme(const char **, program_mode_t *mode) {
    C_ASSERT(mode                   != NULL, false);

    if(mode->compare_function != NULL)
        return false;

    mode->compare_function = string_compare_rhyme;
    return true;
}

bool set_mode_alpha(const char **, program_mode_t *mode) {
    C_ASSERT(mode                   != NULL, false);
    C_ASSERT(mode->compare_function == NULL, false);

    if(mode->compare_function != NULL)
        return false;

    mode->compare_function = string_compare_alphabetic;
    return true;
}

void set_default_options(program_mode_t *mode) {
    C_ASSERT(mode != NULL, );

    if(mode->compare_function == NULL)
        mode->compare_function = DEFAULT_MODE.compare_function;

    if(mode->input_file_name == NULL)
        mode->input_file_name = DEFAULT_MODE.input_file_name;

    if(mode->output_file_name == NULL)
        mode->output_file_name = DEFAULT_MODE.output_file_name;
}
