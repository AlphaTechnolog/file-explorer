#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directives/directives.h"
#include "resources.h"
#include "utils.h"

#define BUF_CHUNK 1024

const char DIR_CHAR = '~';

static char* get_resource_filename(const char* base_filename)
{
    char *result, *prefix;
    size_t length;

    // TODO: Maybe use a compile time directive to override this one
    prefix = "./src/resources/";
    length = strlen(prefix) + strlen(base_filename) + 1;

    result = alloc(length);
    snprintf(result, length, "%s%s", prefix, base_filename);

    return result;
}

char* resource_read(const char* filename)
{
    FILE* fp;
    int cap;
    char read_buf[BUF_CHUNK], *contents, *result, *resource_filename, filename_buf[strlen(filename) + 1];
    strcpy(filename_buf, filename);

    resource_filename = get_resource_filename(filename_buf);

    cap = BUF_CHUNK;
    result = alloc(cap);
    strcpy(result, "");

    if (!(fp = fopen(resource_filename, "r"))) {
        fprintf(stderr, "Unable to include resource_filename %s: ", resource_filename);
        die("fopen");
    }

    free(resource_filename);

    while ((contents = fgets(read_buf, sizeof(read_buf), fp)) != 0) {
        size_t contents_length;

        contents_length = strlen(contents);

        if (contents_length + strlen(result) + 1 > cap) {
            cap = contents_length * BUF_CHUNK;
            result = ralloc(result, cap);
        }

        strncat(result, contents, strlen(contents));
    }

    return result;
}

static int get_index(char* string, char c)
{
    char* e;

    if ((e = strchr(string, c)) == NULL)
        die("strchr");

    return (int)(e - string);
}

static char* expand_directive(char* directive_line)
{
    char contents_buf[strlen(directive_line) - 3], *direc_name, *direc_params, *expansion_result;
    int i = 0, j = 0, sep_index;

    for (; directive_line[i] != '\0'; ++i)
        if (directive_line[i] != DIR_CHAR)
            contents_buf[j++] = directive_line[i];

    contents_buf[j] = '\0';

    sep_index = get_index(contents_buf, ':');

    direc_name = alloc(strlen(contents_buf) + 1);
    direc_params = alloc(strlen(contents_buf) + 1);

    strncpy(direc_name, contents_buf, strlen(contents_buf) + 1);
    direc_name[sep_index] = '\0';

    for (i = sep_index + 1, j = 0; contents_buf[i] != '\0'; ++i)
        direc_params[j++] = contents_buf[i];

    direc_params[j] = '\0';

    expansion_result = directive_resolve_by_name(direc_name, direc_params);

    if (expansion_result == NULL) {
        free(direc_params);
        fprintf(stderr, "[fatal] Invalid given directive '%s'\n", direc_name);
        free(direc_name);
        exit(EXIT_FAILURE);
    }

    free(direc_name);
    free(direc_params);

    return expansion_result;
}

/**
 * Given a template with the include directives, process it so it should read
 * the appropiate resources and include their content in the appropiate string
 * NOTE: These functions may be more suitable to be placed at `src/directives`
 * instead, but, anyways, this is a toy project so :D
 */
char* resource_template_process(char* template_contents)
{
    size_t length = strlen(template_contents) + 1, alloc_cap = BUF_CHUNK;
    char read_buf[length], *cur_ptr, *result;

    strncpy(read_buf, template_contents, length);

    result = alloc(alloc_cap);
    strcpy(result, "");

    for (cur_ptr = strtok(read_buf, "\n"); cur_ptr != NULL; cur_ptr = strtok(NULL, "\n")) {
        char trimmed_line[strlen(cur_ptr) + 1], *directive_result;
        int i, j, directive_start, directive_end;
        size_t line_size, new_length;

        for (i = 0, j = 0; cur_ptr[i] != '\0'; ++i)
            if (cur_ptr[i] != ' ')
                trimmed_line[j++] = cur_ptr[i];

        trimmed_line[j] = '\0';

        line_size = strlen(trimmed_line) - 1;
        directive_start = trimmed_line[0] == DIR_CHAR && trimmed_line[1] == DIR_CHAR;
        directive_end = trimmed_line[line_size] == DIR_CHAR && trimmed_line[line_size - 1] == DIR_CHAR;

        if (!(directive_start && directive_end)) {
            new_length = strlen(result) + strlen(cur_ptr) + 1;
            if (new_length > alloc_cap) {
                alloc_cap = new_length * BUF_CHUNK;
                result = ralloc(result, alloc_cap);
            }

            strncat(result, cur_ptr, new_length);
            strcat(result, "\n");

            continue;
        }

        directive_result = expand_directive(trimmed_line);
        new_length = strlen(result) + strlen(directive_result) + 1;

        if (new_length > alloc_cap) {
            alloc_cap = new_length * BUF_CHUNK;
            result = ralloc(result, alloc_cap);
        }

        strncat(result, directive_result, new_length);
        strcat(result, "\n");

        free(directive_result);
    }

    return result;
}
