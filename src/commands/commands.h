#include <webview/webview.h>

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

struct commands_context
{
    webview_t* w;
};

typedef struct commands_context commands_context_t;

#define DEFINE_COMMAND(name) \
    void (name)(const char* id, const char* req, void* arg)

commands_context_t* commands_context_init(webview_t* w);
void commands_context_deinit(commands_context_t* c);
char* format_error(const char* message);

#endif
