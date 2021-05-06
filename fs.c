#include "fs.h"
#include <stdio.h>
#include <string.h>

void init_fs() {
    static struct node empty[10];

    static struct node node = {
            "/",
            false,
            NULL,
            NULL,
            0,
            empty
    };

    current_node = &node;
    root_node = &node;
}

struct node* get_by_path(const char *path) {
    struct node *result;
    struct node *cur_node;
    int i;
    if (path[0] == '/') {
        cur_node = root_node;
        i = 1;
    } else {
        cur_node = current_node;
        i = 0;
    }

    int name_len = 0;
    char *name = "";
    while (i < strlen(path)) {
        if (path[i] == '/') {
            if (name_len > 0 && i != strlen(path) - 1) {
                for (int j = 0; j < cur_node->children_count; j++) {
                    if (strcmp(cur_node->children[j].name, name) == 0) {
                        cur_node = &cur_node->children[j];
                        break;
                    }
                }
                name_len = 0;
                name = "";
            }
        } else if (path[i] == '.' && i != strlen(path) - 1 && path[i + 1] == '.') {
            cur_node = cur_node->parent;
            i++;
        } else {
            name[name_len] = path[i];
            name_len++;
        }
        i++;
    }

    result = cur_node;

    for (int j = 0; j < result->children_count; j++) {
        if (strcmp(result->children[j].name, name) == 0) {
            result = &result->children[j];
            break;
        }
    }

    return result;
}

char* get_name(const char *path) {
    int name_len = 0;
    int start_i = 0;
    int end_i = 0;
    char *name = "";

    if (path[strlen(path) - 1] == '/') {
        end_i = (int) strlen(path) - 2;
    } else {
        end_i = (int) strlen(path) - 1;
    }

    for (int i = end_i; i >= 0; i--) {
        if (path[i] == '/') {
            start_i = i + 1;
            break;
        }
    }

    for (int i = start_i; i <= end_i; i++) {
        name[name_len] = path[i];
        name_len++;
    }

    return name;
}

void pwd() {
    int nodes_size = 0;
    const char *names[10];
    struct node *cur_node = current_node;

    for (;;) {
        if (strcmp(cur_node->name, "/") != 0) {
            break;
        }
        names[nodes_size++] = cur_node->name;
        cur_node = cur_node->parent;
    }

    printf("/");
    for (int i = nodes_size; i > 0; i--) {
        printf("%s/", names[i]);
    }
    printf("\n");
}

void ls() {
    for (int i = 0; i < current_node->children_count; i++) {
        printf("%s\n", current_node->children[i].name);
    }
}

void cd(const char *path) {
    if (get_by_path(path)->isFile)
        return;

    current_node = get_by_path(path);
}

void mkdir(const char *path) {
    struct node *dir_parent = get_by_path(path);

    if (dir_parent->isFile)
        return;

    struct node empty[10];

    struct node dir = {
            get_name(path),
            false,
            NULL,
            dir_parent,
            0,
            empty
    };

    dir_parent->children[dir_parent->children_count++] = dir;
}

void mkfile(const char *path, const char *content) {
    struct node *file_parent = get_by_path(path);

    if (file_parent->isFile)
        return;

    struct node file = {
            get_name(path),
            true,
            content,
            file_parent,
            0,
            NULL
    };

    file_parent->children[file_parent->children_count++] = file;
}

void show_content(const char *path) {
    struct node *file = get_by_path(path);

    if (!file->isFile) {
        return;
    }

    printf("%s\n", file->content);
}

void rm(const char *path) {
    //todo
}
