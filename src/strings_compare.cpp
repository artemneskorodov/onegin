#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "strings_compare.h"
#include "custom_assert.h"
#include "utils.h"

int string_compare_alphabetic(const void *first, const void *second) {
    C_ASSERT(first  != NULL, 0);
    C_ASSERT(second != NULL, 0);

    const char *first_string  = *(const char *const *)first ;
    const char *second_string = *(const char *const *)second;

    for(; !is_line_end(*first_string); first_string++, second_string++) {
        while(!isalpha(*first_string ) && !is_line_end(*first_string))
            first_string++ ;

        while(!isalpha(*second_string) && !is_line_end(*second_string))
            second_string++;

        int difference = toupper(*first_string) - toupper(*second_string);
        if(difference)
            return difference;
    }
    return toupper(*first_string) - toupper(*second_string);
}

int string_compare_rhyme(const void *first, const void *second) {
    C_ASSERT(first  != NULL, 0);
    C_ASSERT(second != NULL, 0);

    const char *first_string  = *(const char *const *)first ;
    const char *second_string = *(const char *const *)second;

    const char *pointer_first = first_string, *pointer_second = second_string;

    while(!is_line_end(*pointer_first))
        pointer_first++;

    while(!is_line_end(*pointer_second))
        pointer_second++;

    for(; pointer_first != first_string; pointer_first--, pointer_second--) {
        while(!isalpha(*pointer_first) && pointer_first != first_string)
            pointer_first--;

        while(!isalpha(*pointer_second) && pointer_second != second_string)
            pointer_second--;

        if(pointer_second == second_string)
            break;

        int difference = toupper(*pointer_first) - toupper(*pointer_second);
        if(difference != 0)
            return difference;
    }
    return toupper(*pointer_first) - toupper(*pointer_second);
}
