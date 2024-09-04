#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define LIST_CHUNK 1024

list_t* list_init(void)
{
    list_t* l;

    l = alloc(sizeof(list_t));
    l->cap = LIST_CHUNK;
    l->len = 0;
    l->value = alloc(sizeof(void*) * l->cap);

    return l;
}

void list_append(list_t* l, void* val)
{
    l->value[l->len++] = val;
    if (l->len >= l->cap) {
        l->cap *= 2;
        l->value = ralloc(l->value, sizeof(char*) * l->cap);
    }
}

void list_deinit(list_t* l)
{
    ssize_t i;

    for (i = 0; i < l->len; ++i)
        free(l->value[i]);

    free(l->value);
    free(l);
}

void die(const char* prefix)
{
    perror(prefix);
    exit(EXIT_FAILURE);
}

void* alloc(ssize_t len)
{
    void* ptr;

    if (!(ptr = malloc(len)))
        die("malloc");

    return ptr;
}

void* ralloc(void* ptr, ssize_t len)
{
    void* r;

    if (!(r = realloc(ptr, len)))
        die("realloc");

    return r;
}

void iterate_string(char* string, char divider, void (*processor)(char* element, void* parameter), void* parameter)
{
    char* ptr;

    ptr = alloc(strlen(string) + 1);
    strcpy(ptr, string);

    do {
        char buf[strlen(ptr) + 1];
        int i, j;
        if (ptr[0] == divider)
            ptr++;
        for (i = j = 0; ptr[i] != '\0'; ++i) {
            if (ptr[i] == divider)
                break;
            buf[j++] = ptr[i];
        }
        buf[j] = '\0';
        processor(buf, parameter);
        memset(buf, 0, sizeof(buf));
    } while ((ptr = strchr(ptr, divider)) != NULL);

    free(ptr);
}
