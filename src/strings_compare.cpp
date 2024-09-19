#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "strings_compare.h"
#include "custom_assert.h"
#include "utils.h"

int string_compare_alphabetic(const void *first, const void *second) {
    C_ASSERT(first  != NULL, 0);
    C_ASSERT(second != NULL, 0);

    const line_t *first_line  = (const line_t *)first ;
    const line_t *second_line = (const line_t *)second;

    size_t index_first = 0, index_second = 0;

    for( ; index_first < first_line->length; index_first++, index_second++) {
        while(!isalpha(first_line ->start[index_first ]) &&
              index_first  < first_line ->length)
            index_first++ ;

        while(!isalpha(second_line->start[index_second]) &&
              index_second < second_line->length)
            index_second++;

        int difference = toupper(first_line ->start[index_first ]) -
                         toupper(second_line->start[index_second]) ;
        if(difference != 0)
            return difference;
    }
    return toupper(first_line ->start[index_first ]) -
           toupper(second_line->start[index_second]) ;
}

int string_compare_rhyme(const void *first, const void *second) {
    C_ASSERT(first  != NULL, 0);
    C_ASSERT(second != NULL, 0);

    const line_t *first_line  = (const line_t *)first ;
    const line_t *second_line = (const line_t *)second;

    size_t index_first = first_line->length, index_second = second_line->length;

    for( ; index_first != 0; index_first--, index_second--) {
        while(!isalpha(first_line ->start[index_first ]) && index_first  != 0)
            index_first-- ;

        while(!isalpha(second_line->start[index_second]) && index_second != 0)
            index_second--;

        if(index_second == 0 || index_first == 0)
            break;

        int difference = toupper(first_line ->start[index_first ]) -
                         toupper(second_line->start[index_second]) ;
        if(difference != 0)
            return difference;
    }
    return toupper(first_line ->start[index_first ]) -
           toupper(second_line->start[index_second]) ;
}
