#ifndef _DIRECTIVES_H_
#define _DIRECTIVES_H_

struct directive_params
{
    int cap;
    int len;
    char **value;
};

typedef struct directive_params directive_params_t;

enum directive_type
{
	DIRECTIVE_INCLUDE,
	DIRECTIVE_INVALID
};

typedef enum directive_type directive_type_t;

directive_params_t* directive_params_init(void);
void directive_params_from_string(directive_params_t* dp, char* query_string);
void directive_params_append(directive_params_t* dp, char* value);
void directive_params_deinit(directive_params_t* dp);

char* directive_resolve_by_name(char* direc_name, char* direc_params);

#endif
