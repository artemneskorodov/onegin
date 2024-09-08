#include <stdlib.h>
#include "custom_assert.h"
#include "colors.h"

void print_assert_error(const char *string, int line_number, const char *filename) {
    color_printf(RED_TEXT, true, DEFAULT_BACKGROUND,
                 "-<<CUSTOM ASSERT>>-\n"
                 "Caught error on line %d of file \"%s\"\n"
                 "Expression: %s\n", line_number, filename, string);
}
