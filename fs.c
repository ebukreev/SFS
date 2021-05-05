#include "fs.h"
#include <stdlib.h>

struct node {
    char* name;
    bool isFile;
    Node parent;
    Node children[];
};

void init_fs() {
    static struct node node = {
            "root",
            false,
            NULL
    };

    current_node = &node;
}

