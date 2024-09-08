#include "sort.h"
#define NDEBUG
#include "custom_assert.h"

#define CHANGE_VALUES(fisrt, second, changed, element_size, type) \
    while(element_size >= sizeof(type) + changed) {\
        type temp = *(type *)((char *)first + changed);\
        *(type *)((char *)first  + changed) = *(type *)((char *)second + changed);\
        *(type *)((char *)second + changed) = temp;\
        changed += sizeof(type);\
    }\

static void change(void *first, void *second, size_t element_size);

sorting_state_t sort_array(void *base, size_t element_size, size_t elements_number, int (*compare_function)(const void *, const void *)) {
    C_ASSERT(base             != NULL, SORTING_ERROR);
    C_ASSERT(compare_function != NULL, SORTING_ERROR);

    for(size_t unsorted = elements_number; unsorted != 0; unsorted--)
        for(size_t index = 0; index + 1 < unsorted; index++)
            if(compare_function((char *)base + index * element_size, (char *)base + (index + 1) * element_size) > 0)
                change((char *)base + index * element_size, (char *)base + (index + 1) * element_size, element_size);

    return SORTING_SUCCESS;
}

void change(void *first, void *second, size_t element_size) {
    C_ASSERT(first  != NULL, );
    C_ASSERT(second != NULL, );

    size_t changed = 0;
    CHANGE_VALUES(first, second, changed, element_size, long long int);
    CHANGE_VALUES(first, second, changed, element_size, int          );
    CHANGE_VALUES(first, second, changed, element_size, char         );
}
