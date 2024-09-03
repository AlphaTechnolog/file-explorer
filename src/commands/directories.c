#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <cJSON.h>
#include <webview/webview.h>

#include "commands.h"
#include "directories.h"

void obtain_directories_impl(const char* id, const char* req, void* arg)
{
    commands_context_t* c;

    // cJSON* root, *directories, *files;

    c = (commands_context_t*)arg;
    assert(c);
    
    // root = cJSON_CreateObject();
    // files = cJSON_CreateArray();
    // directories = cJSON_CreateArray();

    // cJSON_AddItemToObject(root, "directories", directories);
    // cJSON_AddItemToObject(root, "files", files);

    webview_return(*c->w, id, 0, "{\"content\": \"world\"}");
}
