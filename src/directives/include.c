#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../resources.h"
#include "../utils.h"
#include "directives.h"
#include "include.h"

char* directive_include_expand(directive_params_t* dp)
{
    char *cursor, *result;
    int i, result_cap;

    result_cap = 1024;
    result = alloc(result_cap);
    strcpy(result, "");

    for (i = 0, cursor = dp->value[i]; i < dp->len; cursor = dp->value[++i]) {
        char* resource_contents;
        size_t new_length;

        resource_contents = resource_read(cursor);
        assert(resource_contents != NULL);

        new_length = strlen(result) + strlen(resource_contents) + 1;

        if (new_length > result_cap) {
            result_cap = new_length * 1024;
            result = ralloc(result, result_cap);
        }

        strncat(result, resource_contents, new_length);
        strcat(result, "\n");

        free(resource_contents);
    }

    return result;
}
