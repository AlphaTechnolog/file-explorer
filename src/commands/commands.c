#include <stdlib.h>
#include <webview/webview.h>

#include "../utils.h"
#include "commands.h"

commands_context_t* commands_context_init(webview_t* w)
{
    commands_context_t* c;

    c = alloc(sizeof(commands_context_t));
    c->w = w;

    return c;
}

void commands_context_deinit(commands_context_t* c) { free(c); }
