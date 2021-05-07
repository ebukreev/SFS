#ifndef SFS_FS_H
#define SFS_FS_H

#include <stdbool.h>

typedef struct node {
    const char *name;
    bool isFile;
    const char *content;
    struct node *parent;
    unsigned children_size;
    unsigned children_count;
    struct node *children;
} Node;

Node *current_node;
Node *root_node;

void init_fs();

const char *pwd();

const char *ls();

void cd(const char *path);

void mkdir(const char *path);

void mkfile(const char *path, const char *content);

const char *show_content(const char *path);

void rm(const char *path);

#endif //SFS_FS_H
