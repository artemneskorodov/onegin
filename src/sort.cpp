#include "sort.h"
#include "custom_assert.h"
#include "utils.h"

#define SWAP_VALUES(fisrt, second, swapped, element_size, type)                 \
    while(element_size >= sizeof(type) + swapped) {                               \
        type temp                           = *(type *)((char *)first  + swapped);\
        *(type *)((char *)first  + swapped) = *(type *)((char *)second + swapped);\
        *(type *)((char *)second + swapped) = temp;                               \
        swapped += sizeof(type);                                                  \
    }

#define COPY_VALUE(destanation, source, size, copied, type)           \
    while(size > sizeof(type) + copied) {                            \
        *(type *)(destanation + copied) = *(type *)(source + copied);\
        copied += sizeof(type);                                      \
    }


static const size_t DEFAULT_PIVOT_STORAGE_SIZE = 256;

static void swap(void *first, void *second, size_t element_size);
static void make_pivot_copy(char * pivot_storage  ,
                            char * base           ,
                            size_t element_size   ,
                            size_t elements_number);
static sorting_state_t quick_sort(char *             base            ,
                                  size_t             element_size    ,
                                  size_t             elements_number ,
                                  compare_function_t compare_function,
                                  char *             pivot_storage   );
static size_t make_partition(char *             base            ,
                             size_t             element_size    ,
                             size_t             elements_number ,
                             compare_function_t compare_function,
                             char *             pivot_storage   );
static void copy_element(char *destanation, char *source, size_t size);

void swap(void *first, void *second, size_t element_size) {
    C_ASSERT(first  != NULL, );
    C_ASSERT(second != NULL, );

    size_t swapped = 0;
    SWAP_VALUES(first, second, swapped, element_size, long long int);
    SWAP_VALUES(first, second, swapped, element_size, int          );
    SWAP_VALUES(first, second, swapped, element_size, short        );
    SWAP_VALUES(first, second, swapped, element_size, char         );
}

sorting_state_t sort_array(void *             base            ,
                           size_t             element_size    ,
                           size_t             elements_number ,
                           compare_function_t compare_function) {
    C_ASSERT(base             != NULL, SORTING_ERROR);
    C_ASSERT(compare_function != NULL, SORTING_ERROR);

    char static_pivot_storage[DEFAULT_PIVOT_STORAGE_SIZE] = {};
    char *pivot_storage_pointer = static_pivot_storage;
    if(element_size > DEFAULT_PIVOT_STORAGE_SIZE) {
        pivot_storage_pointer = (char *)calloc(element_size, 1);
        if(pivot_storage_pointer == NULL)
            return SORTING_ERROR;
    }

    sorting_state_t sorting_state = quick_sort((char *)base          ,
                                                element_size         ,
                                                elements_number      ,
                                                compare_function     ,
                                                pivot_storage_pointer);

    if(pivot_storage_pointer != static_pivot_storage)
        free(pivot_storage_pointer);

    return sorting_state;
}

size_t make_partition(char *             base            ,
                      size_t             element_size    ,
                      size_t             elements_number ,
                      compare_function_t compare_function,
                      char *             pivot_storage   ) {
    C_ASSERT(base             != NULL, elements_number);
    C_ASSERT(compare_function != NULL, elements_number);
    C_ASSERT(pivot_storage    != NULL, elements_number);

    make_pivot_copy(pivot_storage, base, element_size, elements_number);

    size_t left_index = 0, right_index = elements_number - 1;

    while(true) {
        C_ASSERT(left_index  < elements_number, elements_number);
        C_ASSERT(right_index < elements_number, elements_number);

        while(compare_function(base + element_size * left_index ,
                               pivot_storage                    ) < 0) {
            left_index++ ;
            C_ASSERT(left_index  < elements_number, elements_number);
        }
        while(compare_function(base + element_size * right_index,
                               pivot_storage                    ) > 0) {
            right_index--;
            C_ASSERT(right_index < elements_number, elements_number);
        }

        C_ASSERT(left_index  < elements_number, elements_number);
        C_ASSERT(right_index < elements_number, elements_number);

        if(left_index >= right_index)
            return right_index;

        swap(base + element_size * left_index ,
             base + element_size * right_index,
             element_size);

        left_index ++;
        right_index--;
    }
}

sorting_state_t quick_sort(char *             base            ,
                           size_t             element_size    ,
                           size_t             elements_number ,
                           compare_function_t compare_function,
                           char *             pivot_storage   ) {
    C_ASSERT(base             != NULL, SORTING_ERROR);
    C_ASSERT(compare_function != NULL, SORTING_ERROR);
    C_ASSERT(pivot_storage    != NULL, SORTING_ERROR);

    if(elements_number < 2)
        return SORTING_SUCCESS;

    size_t partition_index = make_partition(base            ,
                                            element_size    ,
                                            elements_number ,
                                            compare_function,
                                            pivot_storage   );
    if(partition_index >= elements_number)
        return SORTING_ERROR;

    if(quick_sort(base                                       ,
                  element_size                               ,
                  partition_index + 1                        ,
                  compare_function, pivot_storage            ) != SORTING_SUCCESS)
        return SORTING_ERROR;

    if(quick_sort(base + (partition_index + 1) * element_size,
                  element_size                               ,
                  elements_number - partition_index - 1      ,
                  compare_function, pivot_storage            ) != SORTING_SUCCESS)
        return SORTING_ERROR;

    return SORTING_SUCCESS;
}

void make_pivot_copy(char * pivot_storage  ,
                     char * base           ,
                     size_t element_size   ,
                     size_t elements_number) {
    C_ASSERT(pivot_storage != NULL, );
    C_ASSERT(base          != NULL, );

    size_t pivot_index = rand_index(elements_number);
    copy_element(pivot_storage, base + element_size * pivot_index, element_size);
}

void copy_element(char *destanation, char *source, size_t size) {
    C_ASSERT(destanation != NULL, );
    C_ASSERT(source      != NULL, );

    size_t copied = 0;
    COPY_VALUE(destanation, source, size, copied, long long int);
    COPY_VALUE(destanation, source, size, copied, int          );
    COPY_VALUE(destanation, source, size, copied, short        );
    COPY_VALUE(destanation, source, size, copied, char         );
}
