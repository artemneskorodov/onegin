#include "sort.h"
#include "custom_assert.h"
#include "utils.h"

#define CHANGE_VALUES(fisrt, second, changed, element_size, type) \
    while(element_size >= sizeof(type) + changed) {\
        type temp = *(type *)((char *)first + changed);\
        *(type *)((char *)first  + changed) = *(type *)((char *)second + changed);\
        *(type *)((char *)second + changed) = temp;\
        changed += sizeof(type);\
    }\

static void change(void *first, void *second, size_t element_size);

void change(void *first, void *second, size_t element_size) {
    C_ASSERT(first  != NULL, );
    C_ASSERT(second != NULL, );

    size_t changed = 0;
    CHANGE_VALUES(first, second, changed, element_size, long long int);
    CHANGE_VALUES(first, second, changed, element_size, int          );
    CHANGE_VALUES(first, second, changed, element_size, char         );
}

sorting_state_t sort_array(void *base, size_t element_size, size_t elements_number,
                           int (*compare_function)(const void *, const void *)) {

    C_ASSERT(base             != NULL, SORTING_ERROR);
    C_ASSERT(compare_function != NULL, SORTING_ERROR);

    if(elements_number < 2)
        return SORTING_SUCCESS;

    size_t pivot_index = rand_index(elements_number);
    size_t left = 0, right = elements_number - 1;
    while(true) {
        C_ASSERT(left  < elements_number, SORTING_ERROR);
        C_ASSERT(right < elements_number, SORTING_ERROR);
        while(compare_function((char *)base + element_size * left , (char *)base + element_size * pivot_index) < 0) {
            left++ ;
            C_ASSERT(left  < elements_number, SORTING_ERROR);
        }
        while(compare_function((char *)base + element_size * right, (char *)base + element_size * pivot_index) > 0) {
            right--;
            C_ASSERT(right < elements_number, SORTING_ERROR);
        }
        if(left >= right)
            break;

        C_ASSERT(left  < elements_number, SORTING_ERROR);
        C_ASSERT(right < elements_number, SORTING_ERROR);

        change((char *)base + element_size * left, (char *)base + element_size * right, element_size);
        left++ ;
        right--;
    }

    if(sort_array(base, element_size, right, compare_function) != SORTING_SUCCESS)
        return SORTING_ERROR;

    if(sort_array((char *)base + (right + 1) * element_size, element_size,
                  elements_number - right - 1, compare_function) != SORTING_SUCCESS)
        return SORTING_ERROR;

    return SORTING_SUCCESS;
}
