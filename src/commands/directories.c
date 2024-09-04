#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cJSON.h>
#include <webview/webview.h>

#include "../utils.h"
#include "commands.h"
#include "directories.h"

struct directories_scan
{
    list_t* files;
    list_t* directories;
    size_t count;
};

typedef struct directories_scan directories_scan_t;

static directories_scan_t* directories_scan_init(char* dirname)
{
    DIR* dir;
    struct dirent* d;
    directories_scan_t* ds;
    list_t *filenames, *dirnames;

    dir = opendir(dirname);

    if (errno == ENOENT) {
        return NULL;
    }

    filenames = list_init();
    dirnames = list_init();

    ds = alloc(sizeof(directories_scan_t));
    ds->count = 0;
    ds->files = filenames;
    ds->directories = dirnames;

    while ((d = readdir(dir)) != NULL) {
        char* value;
        value = alloc(strlen(d->d_name) + 1);
        strcpy(value, d->d_name);

        if (d->d_type == DT_REG) {
            list_append(ds->files, (void*)value);
        } else if (d->d_type == DT_DIR && strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
            // ignores . and ..
            list_append(ds->directories, (void*)value);
        } else {
            continue;
        }

        ds->count++;
    }

    closedir(dir);

    return ds;
}

static char* directories_scan_serialize(directories_scan_t* scan)
{
    cJSON *root, *files, *directories, *total;
    char* serialized;
    ssize_t i;

    i = 0;

    root = cJSON_CreateObject();
    directories = cJSON_CreateArray();
    files = cJSON_CreateArray();
    total = cJSON_CreateNumber(scan->count);

    cJSON_AddItemToObject(root, "directories", directories);
    cJSON_AddItemToObject(root, "files", files);
    cJSON_AddItemToObject(root, "total", total);

    for (; i < scan->files->len; ++i) {
        cJSON* string;
        string = cJSON_CreateString((char*)scan->files->value[i]);
        cJSON_AddItemToArray(files, string);
    }

    for (i = 0; i < scan->directories->len; ++i) {
        cJSON* string;
        string = cJSON_CreateString((char*)scan->directories->value[i]);
        cJSON_AddItemToArray(directories, string);
    }

    if (!(serialized = cJSON_Print(root))) {
        cJSON_Delete(root);
        die("cJSON_Print()");
        exit(1);
    }

    cJSON_Delete(root);

    return serialized;
}

static void directories_scan_deinit(directories_scan_t* ds)
{
    list_deinit(ds->directories);
    list_deinit(ds->files);
    free(ds);
}

void obtain_directories_impl(const char* id, const char* req, void* arg)
{
    commands_context_t* c;
    cJSON *arguments, *argument;
    char* serialized_scan;
    directories_scan_t* ds;

    c = (commands_context_t*)arg;
    assert(c);

    arguments = cJSON_Parse(req);

    if (!(argument = cJSON_GetArrayItem(arguments, 0)) || !cJSON_IsString(argument)) {
        webview_return(*c->w, id, 1, format_error("Argument is not of type string!"));
        return;
    }

    ds = directories_scan_init(argument->valuestring);
    serialized_scan = directories_scan_serialize(ds);

    webview_return(*c->w, id, 0, serialized_scan);

    directories_scan_deinit(ds);
    cJSON_Delete(arguments);

    free(serialized_scan);
}

static int is_valid_dirname(const char* dirname)
{
    DIR* dir;

    /* seems to segfault somewhere if one does something like /home/user/../.config/ */
    if (strstr(dirname, "..") != NULL) {
        return 0;
    }

    if ((dir = opendir(dirname))) {
        closedir(dir);
        return 1;
    }

    return 0;
}

void check_dirname_impl(const char* id, const char* req, void* arg)
{
    commands_context_t* c;
    cJSON *res_root, *arg_root, *arg_string, *valid_dirname;
    char* serialized_response;

    c = (commands_context_t*)arg;
    assert(c);

    arg_root = cJSON_Parse(req);

    if (!(arg_string = cJSON_GetArrayItem(arg_root, 0))) {
        webview_return(*c->w, id, 1, format_error("Invalid arguments!"));
        cJSON_Delete(arg_root);
        return;
    }

    if (!cJSON_IsString(arg_string)) {
        webview_return(*c->w, id, 1, format_error("Argument is not of type string"));
        cJSON_Delete(arg_root);
        return;
    }

    res_root = cJSON_CreateObject();
    valid_dirname = cJSON_CreateBool(is_valid_dirname(arg_string->valuestring));

    cJSON_AddItemToObject(res_root, "valid", valid_dirname);

    if (!(serialized_response = cJSON_Print(res_root))) {
        die("cJSON_Print()");
        exit(1);
    }

    webview_return(*c->w, id, 0, serialized_response);

    cJSON_Delete(res_root);
    cJSON_Delete(arg_root);

    free(serialized_response);
}
