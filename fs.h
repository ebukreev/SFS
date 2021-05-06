#ifndef SFS_FS_H
#define SFS_FS_H

#include <stdbool.h>

struct node {
    const char *name;
    bool isFile;
    const char *content;
    struct node *parent;
    unsigned children_count;
    struct node *children;
};

struct node *current_node;
struct node *root_node;

void init_fs();

void pwd();

void ls();

void cd(const char *path);

void mkdir(const char *path);

void mkfile(const char *path, const char *content);

void show_content(const char *path);

void rm(const char *path);

#endif //SFS_FS_H
