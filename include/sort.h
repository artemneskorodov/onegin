#ifndef SORT_H
#define SORT_H

#include <stdio.h>

enum sorting_state_t {
    SORTING_SUCCESS,
    SORTING_ERROR
};

sorting_state_t sort_array(void *base, size_t element_size, size_t elements_number, int (*compare_function)(const void *, const void *));

#endif
