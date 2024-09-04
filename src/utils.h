#include <sys/types.h>

#ifndef UTILS_H_
#define UTILS_H_

struct list
{
    ssize_t len;
    ssize_t cap;
    void** value;
};

typedef struct list list_t;

list_t* list_init(void);
void list_append(list_t* l, void* val);
void list_deinit(list_t* l);

void die(const char* prefix);
void* alloc(ssize_t len);
void* ralloc(void* ptr, ssize_t len);
void iterate_string(char* string, char divider, void (*processor)(char* element, void* parameter), void* parameter);

#endif
