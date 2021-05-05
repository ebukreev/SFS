#include "fs.h"
#include <stdio.h>

void init_fs() {
    static struct node empty[10];

    static struct node node = {
            "root",
            false,
            NULL,
            0,
            empty
    };

    current_node = &node;
}

void ls() {
    if (current_node->isFile)
        return;

    for (int i = 0; i < current_node->children_count; i++) {
            printf("%s\n", current_node->children[i].name);
    }
}

void mkdir(const char *name) {
    if (current_node->isFile)
        return;

    struct node empty[10];

    struct node dir = {
            name,
            false,
            NULL,
            0,
            empty
    };

    current_node->children[current_node->children_count]= dir;
    current_node->children_count++;
}
