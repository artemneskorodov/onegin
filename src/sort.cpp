#include "sort.h"
#include "custom_assert.h"
#include "utils.h"

#define CHANGE_VALUES(fisrt, second, changed, element_size, type) \
    while(element_size >= sizeof(type) + changed) {\
        type temp = *(type *)((char *)first + changed);\
        *(type *)((char *)first  + changed) = *(type *)((char *)second + changed);\
        *(type *)((char *)second + changed) = temp;\
        changed += sizeof(type);\
    }

static void change(void *first, void *second, size_t element_size);
static size_t make_partition(void *base, size_t element_size, size_t elements_number,
                             int (*compare_function)(const void *, const void *));

void change(void *first, void *second, size_t element_size) {
    C_ASSERT(first  != NULL, );
    C_ASSERT(second != NULL, );

    size_t changed = 0;
    CHANGE_VALUES(first, second, changed, element_size, long long int);
    CHANGE_VALUES(first, second, changed, element_size, int          );
    CHANGE_VALUES(first, second, changed, element_size, char         );
}

sorting_state_t sort_array(void *base, size_t element_size, size_t elements_number,
                           int (*compare_function)(const void *, const void *)     ) {
    C_ASSERT(base             != NULL, SORTING_ERROR);
    C_ASSERT(compare_function != NULL, SORTING_ERROR);

    if(elements_number < 2)
        return SORTING_SUCCESS;

    size_t partition_index = make_partition(base, element_size, elements_number, compare_function);
    if(partition_index >= elements_number)
        return SORTING_SUCCESS;

    if(sort_array((char *)base, element_size,
                  partition_index, compare_function) != SORTING_SUCCESS)
        return SORTING_ERROR;

    if(sort_array((char *)base + (partition_index + 1) * element_size, element_size,
                  elements_number - partition_index - 1, compare_function) != SORTING_SUCCESS)
        return SORTING_ERROR;

    return SORTING_SUCCESS;
}

size_t make_partition(void *base, size_t element_size, size_t elements_number,
                      int (*compare_function)(const void *, const void *)) {
    C_ASSERT(base             != NULL, elements_number);
    C_ASSERT(compare_function != NULL, elements_number);
    size_t pivot_index = rand_index(elements_number);
    size_t left_index = 0, right_index = elements_number - 1;

    while(true) {
        C_ASSERT(left_index  < elements_number, elements_number);
        C_ASSERT(right_index < elements_number, elements_number);

        while(compare_function((char *)base + element_size * left_index ,
                               (char *)base + element_size * pivot_index) < 0) {
            left_index++ ;
            C_ASSERT(left_index  < elements_number, elements_number);
        }
        while(compare_function((char *)base + element_size * right_index,
                               (char *)base + element_size * pivot_index) > 0) {
            right_index--;
            C_ASSERT(right_index < elements_number, elements_number);
        }
        if(left_index >= right_index)
            return right_index;

        C_ASSERT(left_index  < elements_number, elements_number);
        C_ASSERT(right_index < elements_number, elements_number);

        change((char *)base + element_size * left_index, (char *)base + element_size * right_index, element_size);
        left_index++;
        right_index--;
    }
}
