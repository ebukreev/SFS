#ifndef SFS_FS_H
#define SFS_FS_H

#include <stdbool.h>

struct node;
typedef struct node* Node;

typedef union {
    char* dir_content[];
    char file_content[];
} node_content;

Node init_fs();

#endif //SFS_FS_H
