#include <sys/types.h>

#ifndef UTILS_H_
#define UTILS_H_

void die(const char* prefix);
void* alloc(ssize_t len);
void* ralloc(void* ptr, ssize_t len);
void iterate_string(char* string, char divider, void (*processor)(char* element, void* parameter), void* parameter);

#endif
