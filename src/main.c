#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webview/webview.h>

#include "resources.h"
#include "utils.h"

#include "commands/commands.h"
#include "commands/directories.h"

struct resources
{
    char* html_contents;
    char* processed_template;
};

typedef struct resources resources_t;

static resources_t* resources_init(void)
{
    resources_t* r;

    r = alloc(sizeof(resources_t));

    r->html_contents = resource_read("index.html");
    r->processed_template = resource_template_process(r->html_contents);

    return r;
}

static void resources_load(resources_t* r, webview_t* w)
{
    assert(strcmp(r->processed_template, "") != 0);
    webview_set_html(*w, r->processed_template);
}

static void resources_deinit(resources_t* r)
{
    free(r->html_contents);
    free(r->processed_template);
    free(r);
}

static void display_impl(const char* id, const char* req, void* arg) { printf("[client] %s\n", req); }


static void load_bindings(commands_context_t* c)
{
    webview_bind(*c->w, "display", display_impl, NULL);
    webview_bind(*c->w, "obtainUsername", obtain_username_impl, (void*)c);
    webview_bind(*c->w, "obtainDirectories", obtain_directories_impl, (void*)c);
}

int main(int argc, char* argv[])
{
    webview_t w;
    resources_t* r;
    commands_context_t* c;

    r = resources_init();
    w = webview_create(0, NULL);
    c = commands_context_init(&w);

    webview_set_title(w, "File Explorer");
    webview_set_size(w, 800, 600, WEBVIEW_HINT_NONE);
    load_bindings(c);
    resources_load(r, &w);
    webview_run(w);
    webview_destroy(w);

    resources_deinit(r);

    return 0;
}
