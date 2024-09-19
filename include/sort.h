#ifndef SORT_H
#define SORT_H

#include <stdio.h>

enum sorting_state_t {
    SORTING_SUCCESS,
    SORTING_ERROR
};

typedef int (*compare_function_t)(const void *, const void *);

sorting_state_t sort_array(void *             base            ,
                           size_t             element_size    ,
                           size_t             elements_number ,
                           compare_function_t compare_function);

#endif
