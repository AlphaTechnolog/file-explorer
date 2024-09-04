#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
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

void commands_context_deinit(commands_context_t* c)
{
    free(c);
}

char* format_error(const char* message)
{
    char msg_buf[strlen(message) + 1], *formatted_json;
    cJSON *root, *json_message;

    strcpy(msg_buf, message);

    root = cJSON_CreateObject();
    json_message = cJSON_CreateString(msg_buf);

    cJSON_AddItemToObject(root, "error", json_message);

    if (!(formatted_json = cJSON_Print(root))) {
        cJSON_Delete(root);
        die("cJSON_Print");
        exit(1);
    }

    cJSON_Delete(root);

    return formatted_json;
}