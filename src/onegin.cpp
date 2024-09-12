#include <stdlib.h>
#include <string.h>
#include "onegin.h"
#include "custom_assert.h"
#include "colors.h"
#include "sort.h"
#include "strings_compare.h"
#include "utils.h"

const char *ORIGINAL_OUTPUT   = "OneginOriginal.txt"  ;
const char *ALPHABETIC_OUTPUT = "OneginAlphabetic.txt";
const char *RHYME_OUTPUT      = "OneginRhyme.txt"     ;

void free_text(onegin_text_t *text) {
    C_ASSERT(text != NULL, );

    free(text->input_text);
    free(text->lines     );

    text->input_text = NULL;
    text->lines      = NULL;
}

exit_code_t try_read_file(onegin_text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    switch(read_file(text)) {
        case UNKNOWN_READING_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Unexpected error while reading from file '%s'.\n",
                         text->filename);
            free_text(text);
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
            free_text(text);
            return EXIT_CODE_FAILURE;
        }
        case READING_ALLOCATION_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Error while allocation memory to input buffer.\n");
            free_text(text);
            return EXIT_CODE_FAILURE;
        }
        default: {
            C_ASSERT(false, EXIT_CODE_FAILURE);
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t try_parse_text(onegin_text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    switch(parse_lines(text)) {
        case UNKNOWN_PARSING_ERROR: {
            color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                         "Unexpected error while parsing input.\n");
            free_text(text);
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
            free_text(text);
            return EXIT_CODE_FAILURE;
        }
        default: {
            C_ASSERT(false, EXIT_CODE_FAILURE);
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t try_sort_alphabetic(onegin_text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    if(sort_array(text->lines, sizeof(char *), text->lines_number, string_compare_alphabetic) != SORTING_SUCCESS) {
        free_text(text);
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while sorting alphabetic.\n");
        return EXIT_CODE_FAILURE;
    }

    if(write_file(ALPHABETIC_OUTPUT, text) != WRITING_SUCCESS) {
        free_text(text);
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while writing result of alphabetical sorting in file.\n");
        return EXIT_CODE_FAILURE;
    }

    color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                 "Successfully sorted alphabetically.\n");
    return EXIT_CODE_SUCCESS;
}

exit_code_t try_sort_rhyme(onegin_text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    if(sort_array(text->lines, sizeof(char *), text->lines_number, string_compare_rhyme) != SORTING_SUCCESS) {
        free_text(text);
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while sorting from end.\n");
        return EXIT_CODE_FAILURE;
    }

    if(write_file(RHYME_OUTPUT, text) != WRITING_SUCCESS) {
        free_text(text);
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while writing result of sorting from end in file.\n");
        return EXIT_CODE_FAILURE;
    }

    color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                 "Successfully sorted from end.\n");
    return EXIT_CODE_SUCCESS;
}

exit_code_t try_print_original(onegin_text_t *text) {
    C_ASSERT(text != NULL, EXIT_CODE_FAILURE);

    FILE *output = fopen(ORIGINAL_OUTPUT, "wb");
    if(output == NULL) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while oppening file '%s'.\n",
                     ORIGINAL_OUTPUT);
        free_text(text);
        return EXIT_CODE_SUCCESS;
    }

    size_t text_length = strlen(text->input_text);
    if(fwrite(text->input_text, sizeof(char), text_length, output) != text_length) {
        color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                     "Error while writing original text to file '%s'.\n",
                     ORIGINAL_OUTPUT);
        free_text(text);
        return EXIT_CODE_SUCCESS;
    }
    color_printf(GREEN_TEXT, true, DEFAULT_BACKGROUND,
                 "Successfully written original text to file '%s'.\n",
                 ORIGINAL_OUTPUT);
    return EXIT_CODE_SUCCESS;
}

void clean_onegin(void) {
    if(remove(ALPHABETIC_OUTPUT) == 0)
        color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND,
                     "Removed '%s'.\n", ALPHABETIC_OUTPUT);
    if(remove(RHYME_OUTPUT     ) == 0)
        color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND,
                     "Removed '%s'.\n", RHYME_OUTPUT     );
    if(remove(ORIGINAL_OUTPUT  ) == 0)
        color_printf(YELLOW_TEXT, true, DEFAULT_BACKGROUND,
                     "Removed '%s'.\n", ORIGINAL_OUTPUT  );
}
