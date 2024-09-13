#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "onegin.h"
#include "custom_assert.h"
#include "colors.h"
#include "sort.h"
#include "strings_compare.h"
#include "utils.h"

static const char *DEFAULT_READ_FILE_NAME = "Onegin.txt"          ;
static const char *ORIGINAL_OUTPUT        = "OneginOriginal.txt"  ;
static const char *ALPHABETIC_OUTPUT      = "OneginAlphabetic.txt";
static const char *RHYME_OUTPUT           = "OneginRhyme.txt"     ;

static bool clean_if_needed(const char *param);

void free_text(text_t *text) {
    C_ASSERT(text != NULL, );

    free(text->input_text);
    free(text->lines     );
    text->input_text = NULL;
    text->lines      = NULL;
}

exit_code_t try_read_file(text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    switch(read_file(text)) {
        case UNKNOWN_READING_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Unexpected error while reading from file '%s'.\n",
                         text->filename);
            return EXIT_CODE_FAILURE;
        }
        case READING_SUCCESS: {
            color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                         "Successfully read text of Onegin from file '%s'.\n",
                         text->filename);
            return EXIT_CODE_SUCCESS;
        }
        case READING_NO_SUCH_FILE: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "There is no file '%s'.\n",
                         text->filename);
            return EXIT_CODE_FAILURE;
        }
        case READING_ALLOCATION_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Error while allocation memory to input buffer.\n");
            return EXIT_CODE_FAILURE;
        }
        default: {
            C_ASSERT(false, EXIT_CODE_FAILURE);
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t try_parse_text(text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    switch(parse_lines(text)) {
        case UNKNOWN_PARSING_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Unexpected error while parsing input.\n");
            return EXIT_CODE_FAILURE;
        }
        case PARSING_SUCCESS: {
            color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                         "Successfully parsed input.\n");
            return EXIT_CODE_SUCCESS;
        }
        case PARSING_ALLOCATION_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Error while allocating memory to array of pointers.\n");
            return EXIT_CODE_FAILURE;
        }
        default: {
            C_ASSERT(false, EXIT_CODE_FAILURE);
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t try_sort_alphabetic(text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    if(sort_array(text->lines, sizeof(char *), text->lines_number, string_compare_alphabetic) != SORTING_SUCCESS) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while sorting alphabetic.\n");
        return EXIT_CODE_FAILURE;
    }

    if(write_file(ALPHABETIC_OUTPUT, text) != WRITING_SUCCESS) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while writing result of alphabetical sorting in file.\n");
        return EXIT_CODE_FAILURE;
    }

    color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                 "Successfully sorted alphabetically.\n");
    return EXIT_CODE_SUCCESS;
}

exit_code_t try_sort_rhyme(text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    if(sort_array(text->lines, sizeof(char *), text->lines_number, string_compare_rhyme) != SORTING_SUCCESS) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while sorting from end.\n");
        return EXIT_CODE_FAILURE;
    }

    if(write_file(RHYME_OUTPUT, text) != WRITING_SUCCESS) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while writing result of sorting from end in file.\n");
        return EXIT_CODE_FAILURE;
    }

    color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                 "Successfully sorted from end\n");
    return EXIT_CODE_SUCCESS;
}

exit_code_t try_print_original(text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    FILE *output = fopen(ORIGINAL_OUTPUT, "wb");
    if(output == NULL) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while oppening file '%s'.\n",
                     ORIGINAL_OUTPUT);
        return EXIT_CODE_SUCCESS;
    }

    size_t text_length = strlen(text->input_text);
    if(fwrite(text->input_text, sizeof(char), text_length, output) != text_length) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while writing original text to file '%s'.\n",
                     ORIGINAL_OUTPUT);
        return EXIT_CODE_SUCCESS;
    }

    color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                 "Successfully written original text to file '%s'.\n",
                 ORIGINAL_OUTPUT);
    return EXIT_CODE_SUCCESS;
}

bool clean_if_needed(const char *param) {
    if(strcmp(param, "--clean") != 0)
        return false;

    if(remove(ALPHABETIC_OUTPUT) == 0)
        color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND,
                     "Deleted '%s'.\n", ALPHABETIC_OUTPUT);
    if(remove(RHYME_OUTPUT     ) == 0)
        color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND,
                     "Deleted '%s'.\n", RHYME_OUTPUT     );
    if(remove(ORIGINAL_OUTPUT  ) == 0)
        color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND,
                     "Deleted '%s'.\n", ORIGINAL_OUTPUT  );
    return true;
}

parsing_input_exit_code_t parse_input(text_t *text, const int argc, const char *argv[]) {
    C_ASSERT(text != NULL, PARSING_INPUT_ERROR);
    C_ASSERT(argv != NULL, PARSING_INPUT_ERROR);

    if(argc == 1) {
        text->filename = DEFAULT_READ_FILE_NAME;
        return PARSING_INPUT_SUCCESS;
    }
    else if(argc == 2) {
        if(clean_if_needed(argv[1]) == true)
            return PARSING_INPUT_EXIT;
        text->filename = argv[1];
        return PARSING_INPUT_SUCCESS;
    }
    else if(argc > 2)
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Unexpected parameter '%s'.\n",
                     argv[2]);
    return PARSING_INPUT_ERROR;
}
