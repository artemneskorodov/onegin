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
static size_t make_partition(char *base, size_t element_size, size_t elements_number,
                             compare_function_t compare_function);
static char *make_pivot_copy(char *base, size_t element_size, size_t elements_number);

void change(void *first, void *second, size_t element_size) {
    C_ASSERT(first  != NULL, );
    C_ASSERT(second != NULL, );

    size_t changed = 0;
    CHANGE_VALUES(first, second, changed, element_size, long long int);
    CHANGE_VALUES(first, second, changed, element_size, int          );
    CHANGE_VALUES(first, second, changed, element_size, char         );
}

sorting_state_t sort_array(void *base, size_t element_size, size_t elements_number,
                           compare_function_t compare_function) {
    C_ASSERT(base             != NULL, SORTING_ERROR);
    C_ASSERT(compare_function != NULL, SORTING_ERROR);

    char *base_ = (char *)base;

    if(elements_number < 2)
        return SORTING_SUCCESS;

    size_t partition_index = make_partition(base_, element_size, elements_number, compare_function);
    if(partition_index >= elements_number)
        return SORTING_ERROR;

    if(sort_array(base_, element_size,
                  partition_index + 1, compare_function) != SORTING_SUCCESS)
        return SORTING_ERROR;

    if(sort_array(base_ + (partition_index + 1) * element_size, element_size,
                  elements_number - partition_index - 1, compare_function) != SORTING_SUCCESS)
        return SORTING_ERROR;

    return SORTING_SUCCESS;
}

size_t make_partition(char *base, size_t element_size, size_t elements_number,
                      compare_function_t compare_function) {
    C_ASSERT(base             != NULL, elements_number);
    C_ASSERT(compare_function != NULL, elements_number);

    char *pivot_copy = make_pivot_copy(base, element_size, elements_number);
    if(pivot_copy == NULL)
        return elements_number;

    size_t left_index = 0, right_index = elements_number - 1;

    while(true) {
        C_ASSERT(left_index  < elements_number, elements_number);
        C_ASSERT(right_index < elements_number, elements_number);

        while(compare_function(base + element_size * left_index ,
                               pivot_copy) < 0) {
            left_index++ ;
            C_ASSERT(left_index  < elements_number, elements_number);
        }
        while(compare_function(base + element_size * right_index,
                               pivot_copy) > 0) {
            right_index--;
            C_ASSERT(right_index < elements_number, elements_number);
        }

        C_ASSERT(left_index  < elements_number, elements_number);
        C_ASSERT(right_index < elements_number, elements_number);

        if(left_index >= right_index) {
            free(pivot_copy);
            return right_index;
        }

        change(base + element_size * left_index, base + element_size * right_index, element_size);

        left_index++;
        right_index--;
    }
}

char *make_pivot_copy(char *base, size_t element_size, size_t elements_number) {
    size_t pivot_index = rand_index(elements_number);
    char *pivot = (char *)calloc(element_size, 1);
    if(pivot == NULL)
        return NULL;
    for(size_t pivot_byte = 0; pivot_byte < element_size; pivot_byte++)
        pivot[pivot_byte] = base[element_size * pivot_index + pivot_byte];

    return pivot;
}
