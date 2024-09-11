#include <ctype.h>
#include <stdio.h>
#include "strings_compare.h"
#include "custom_assert.h"

int string_compare_alphabetic(const void *first, const void *second) {
    C_ASSERT(first  != NULL, 0);
    C_ASSERT(second != NULL, 0);

    const char *first_string  = *(const char *const *)first ;
    const char *second_string = *(const char *const *)second;

    for(; *first_string  != '\r' && *first_string != '\n' && *first_string != '\0'; first_string++, second_string++) {
        while(!isalpha(*first_string ) && *first_string  != '\r' && *first_string != '\n' && *first_string != '\0')
            first_string++ ;

        while(!isalpha(*second_string) && *second_string != '\r' && *second_string != '\n' && *second_string != '\0')
            second_string++;

        if(toupper(*first_string) != toupper(*second_string))
            return toupper(*first_string) - toupper(*second_string);
    }
    return toupper(*first_string) - toupper(*second_string);
}

int string_compare_rhyme(const void *first, const void *second) {
    C_ASSERT(first  != NULL, 0);
    C_ASSERT(second != NULL, 0);

    const char *first_string  = *(const char *const *)first ;
    const char *second_string = *(const char *const *)second;

    const char *pointer_first = first_string, *pointer_second = second_string;

    while(*pointer_first != '\r' && *pointer_first != '\n' && *pointer_first != '\0')
        pointer_first++;

    while(*pointer_second != '\r' && *pointer_second != '\n' && *pointer_second != '\0')
        pointer_second++;

    for(; pointer_first != first_string; pointer_first--, pointer_second--) {
        while(!isalpha(*pointer_first) && pointer_first != first_string)
            pointer_first--;

        while(!isalpha(*pointer_second) && pointer_second != second_string)
            pointer_second--;

        if(pointer_second == second_string)
            break;

        if(toupper(*pointer_first) != toupper(*pointer_second))
            return toupper(*pointer_first) - toupper(*pointer_second);
    }
    return toupper(*pointer_first) - toupper(*pointer_second);
}
