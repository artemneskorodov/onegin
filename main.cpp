#include "utils.h"
//TODO add custom assert
//TODO color print
//TODO string comparison from start and from and skipping all characters that are not alphabetic
//TODO sorting (bubble for start, qsort after)
//TODO reading from chosen in argv file

int main(void) {
    char *input = NULL;
    if(read_file("Onegin.txt", &input) != READING_SUCCESS) {
        free(input);
        printf("Reading from Onegin.txt error\n");
        return EXIT_FAILURE;
    }

    parsed_text_t text = {};
    if(parse_strings(input, &text) != PARSING_SUCCESS) {
        free(text->lines);
        free(input);
        printf("Strings parsing error\n");
        return EXIT_FAILURE;
    }


    //TODO sorting with choosing of way with flags (--alphabetic for alphabet sorting and --rhyme for pseudo rhymes)
    //TODO wrighting to console and to file depending on flags


    free(text->lines);
    free(input);
    return EXIT_SUCCESS;
}
