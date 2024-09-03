#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"
#include "directives.h"
#include "include.h"

directive_params_t* directive_params_init(void)
{
    directive_params_t* dp = alloc(sizeof(directive_params_t));

    dp->len = 0;
    dp->cap = 10;
    dp->value = alloc(sizeof(char*) * dp->cap);

    return dp;
}

static void param_processor(char* element, void* parameter)
{
    directive_params_t* dp;
    char* item;

    dp = (directive_params_t*)parameter;
    assert(dp != NULL);

    item = alloc(strlen(element) + 1);
    strncpy(item, element, strlen(element) + 1);
    assert(item != NULL);

    directive_params_append(dp, item);
}

/**
 * example form of query_string is `"hello,world,example"`.
 */
void directive_params_from_string(directive_params_t* dp, char* query_string)
{
    iterate_string(query_string, ',', param_processor, (void*)dp);
}

/**
 * NOTE: `value` here is supposed to be an allocated string so
 * `free()` will be called on it automatically when `directive_params_deinit()`
 * gets called aswell. See `directive_params_from_string()` for easier usage.
 */
void directive_params_append(directive_params_t* dp, char* value)
{
    dp->value[dp->len++] = value;
    if (dp->cap >= dp->len) {
        dp->cap *= 10;
        dp->value = ralloc(dp->value, sizeof(char*) * dp->cap);
    }
}

void directive_params_deinit(directive_params_t* dp)
{
    int i = 0;

    for (; i < dp->len; ++i) {
        free(dp->value[i]);
    }

    free(dp->value);
    free(dp);
}

/* Add more directives if needed here */
static directive_type_t directive_type_from_string(char* input)
{
    if (strcmp(input, "include") == 0) {
        return DIRECTIVE_INCLUDE;
    }

    return DIRECTIVE_INVALID;
}

char* directive_resolve_by_name(char* direc_name, char* direc_params)
{
    directive_params_t* dp;
    directive_type_t directive_id;
    char* resolved_directive;

    dp = directive_params_init();
    directive_params_from_string(dp, direc_params);

    if (dp->len == 0) {
        fprintf(stderr, "Bad syntax in template!\n");
        exit(EXIT_FAILURE);
    }

    assert(dp->len > 0);

    resolved_directive = NULL;
    directive_id = directive_type_from_string(direc_name);

    switch (directive_id) {
    case DIRECTIVE_INCLUDE:
        resolved_directive = directive_include_expand(dp);
        break;
    case DIRECTIVE_INVALID:
        break;
    }

    directive_params_deinit(dp);

    return resolved_directive;
}
