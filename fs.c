#include "fs.h"
#include <stdlib.h>

struct node {
    char* name;
    bool isFile;
    Node parent;
    Node children[];
};

Node init_fs() {
    static struct node node = {
            "root",
            false,
            NULL
    };

    return &node;
}

