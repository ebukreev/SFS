#ifndef SFS_FS_H
#define SFS_FS_H

#include <stdbool.h>

struct node {
    const char* name;
    bool isFile;
    const char* content;
    struct node* parent;
    unsigned children_count;
    struct node* children;
};

struct node* current_node;

void init_fs();

void ls();

void mkdir(const char* name);

#endif //SFS_FS_H
