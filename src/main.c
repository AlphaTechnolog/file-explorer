#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cJSON.h>
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

static void display_impl(const char* id, const char* req, void* arg) {
    commands_context_t* c;
    cJSON* arg_array, *arg_array_item;
    int i;

    i = 0;

    c = (commands_context_t*)arg;
    assert(c);

    printf("\nclient payload:\n");

    if (!cJSON_IsArray((arg_array = cJSON_Parse(req)))) {
        printf("1. %s (unparsed, wanted an array)\n", req);
        webview_return(*c->w, id, 0, "");
        return;
    }

    cJSON_ArrayForEach(arg_array_item, arg_array) {
        printf("%d. ", ++i);
        if (cJSON_IsNumber(arg_array_item)) {
            printf("%.2f\n", arg_array_item->valuedouble);
        } else if (cJSON_IsString(arg_array_item)) {
            printf("%s\n", arg_array_item->valuestring);
        } else if (cJSON_IsBool(arg_array_item)) {
            printf("%s\n", arg_array_item->valueint ? "true" : "false");
        } else {
            printf("skipping\n");
        }
    }

    printf("\n----\n");

    webview_return(*c->w, id, 0, "");

    cJSON_Delete(arg_array);
}

static void obtain_username_impl(const char* id, const char* req, void* arg)
{
    commands_context_t* c;
    char* username, *result_json;
    cJSON* root, *message_string;

    c = (commands_context_t*)arg;
    assert(c);

    if (!(username = getenv("USER"))) {
        webview_return(*c->w, id, 1, format_error("Unable to get $USER"));
        return;
    }

    root = cJSON_CreateObject();
    message_string = cJSON_CreateString(username);

    cJSON_AddItemToObject(root, "username", message_string);

    if (!(result_json = cJSON_Print(root))) {
        cJSON_Delete(root);
        webview_return(*c->w, id, 1, format_error("Unable to format json"));
        return;
    }

    cJSON_Delete(root);
    webview_return(*c->w, id, 0, result_json);
}

#define DOBIND(name, impl) \
    webview_bind(*c->w, name, impl, (void*)c);

static void load_bindings(commands_context_t* c)
{
    DOBIND("display", display_impl);
    DOBIND("obtainUsername", obtain_username_impl);
    DOBIND("obtainDirectories", obtain_directories_impl);
    DOBIND("checkDirname", check_dirname_impl);
}

#ifdef DOBIND
#undef DOBIND
#endif

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
