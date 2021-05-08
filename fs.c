#include "fs.h"
#include <string.h>
#include "malloc.h"

void init_fs() {
    Node *n = (Node *) malloc(sizeof(Node));

    n->name = "/";
    n->isFile = false;
    n->content = NULL;
    n->parent = NULL;
    n->children_size = ARRAY_START_SIZE;
    n->children_count = 0;
    n->children = (Node *) calloc(ARRAY_START_SIZE, sizeof(Node));

    current_node = n;
    root_node = n;
}

Node* get_by_path(const char *path) {
    Node *result;
    Node *cur_node;
    size_t i;
    if (path[0] == '/') {
        cur_node = root_node;
        i = 1;
    } else {
        cur_node = current_node;
        i = 0;
    }

    size_t name_len = 0;
    size_t name_size = ARRAY_START_SIZE;
    char *name = calloc(name_size, sizeof (char));
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
                free(name);
                name = calloc(name_size, sizeof (char));
            }
        } else if (path[i] == '.' && i != strlen(path) - 1 && path[i + 1] == '.') {
            cur_node = cur_node->parent;
            i++;
        } else {
            if (name_len + 1 >= name_size) {
                name_size *= 2;
                name = (char *) realloc(name, sizeof (char) * name_size);
            }
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
    size_t name_len = 0;
    size_t start_i = 0;
    int end_i;

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

    char *name = (char *) calloc(end_i - start_i + 2, sizeof (char));

    for (size_t i = start_i; i <= end_i; i++) {
        name[name_len] = path[i];
        name_len++;
    }

    return name;
}

char *get_path(Node *node) {
    int nodes_size = 0;
    size_t names_size = ARRAY_START_SIZE;
    const char **names = (const char **) calloc(names_size, sizeof (*names));
    Node *cur_node = node;
    size_t path_len = 0;

    for (;;) {
        if (strcmp(cur_node->name, "/") == 0) {
            break;
        }
        if (nodes_size + 1 >= names_size) {
            names_size *= 2;
            names = realloc(names, names_size * sizeof (*names));
        }
        names[nodes_size] = calloc(strlen(cur_node->name), sizeof (char));
        names[nodes_size] = cur_node->name;
        nodes_size++;
        path_len += strlen(cur_node->name);
        cur_node = cur_node->parent;
    }

    int result_index = 1;
    char *result = calloc(path_len + nodes_size + 2, sizeof (char));

    result[0] = '/';
    for (int i = nodes_size - 1; i >= 0; i--) {
        for (int j = 0; j < strlen(names[i]); j++) {
            result[result_index++] = names[i][j];
        }
        result[result_index++] = '/';
    }

    return result;
}

const char *pwd() {
    return get_path(current_node);
}

const char *ls() {
    size_t result_size = 0;
    for (int i = 0; i < current_node->children_count; i++) {
        result_size += strlen(current_node->children[i].name) + 1;
    }

    char *result = (char *) calloc(result_size + 1, sizeof (char));
    size_t result_index = 0;

    for (int i = 0; i < current_node->children_count; i++) {
        for (int j = 0; j < strlen(current_node->children[i].name); j++) {
            result[result_index++] = current_node->children[i].name[j];
        }
        result[result_index++] = '\n';
    }

    return result;
}

void cd(const char *path) {
    if (get_by_path(path)->isFile)
        return;

    current_node = get_by_path(path);
}

void add_to_parent(Node *node, Node *parent) {
    if (parent->children_count + 1 >= parent->children_size) {
        parent->children_size *= 2;
        parent->children = (Node *) realloc(parent->children, parent->children_size * sizeof(Node));
    }

    parent->children[parent->children_count++] = *node;
}

void mkdir(const char *path) {
    Node *dir_parent = get_by_path(path);

    if (dir_parent->isFile)
        return;

    Node *dir = (Node *) malloc(sizeof(Node));

    dir->name = get_name(path);
    dir->isFile = false;
    dir->content = NULL;
    dir->parent = dir_parent;
    dir->children_size = ARRAY_START_SIZE;
    dir->children_count = 0;
    dir->children = (Node *) calloc(ARRAY_START_SIZE, sizeof(Node));

    add_to_parent(dir, dir_parent);
}

void mkfile(const char *path, const char *content) {
    Node *file_parent = get_by_path(path);

    if (file_parent->isFile)
        return;

    Node *file = (Node *) malloc(sizeof(Node));

    file->name = get_name(path);
    file->isFile = true;
    file->content = content;
    file->parent = file_parent;
    file->children_size = ARRAY_START_SIZE;
    file->children_count = 0;
    file->children = NULL;

    add_to_parent(file, file_parent);
}

const char *show_content(const char *path) {
    Node *file = get_by_path(path);

    if (!file->isFile) {
        return NULL;
    }

    return file->content;
}

void rm(const char *name) {
    if (strcmp(name, "/") == 0) {
        free(root_node->children);
        free(root_node);
        return;
    }

    bool flag = false;
    for (size_t i = 0; i < current_node->children_count; i++) {
        if (flag) {
            current_node->children[i - 1] = current_node->children[i];
        } else if (strcmp(current_node->children[i].name, name) == 0) {
            flag = true;
        }
    }
    current_node->children_count--;
    if (current_node->children_count == 0) {
       current_node->children = calloc(ARRAY_START_SIZE, sizeof (Node));
    }
    if (current_node->children_count <current_node->children_size / 2 && current_node->children_size > ARRAY_START_SIZE) {
        current_node->children = realloc(current_node->children, current_node->children_size / 2 * sizeof (Node));
    }
}
