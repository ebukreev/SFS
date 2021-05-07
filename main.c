#include <stdio.h>
#include "fs.h"

int main() {
    init_fs();
    mkdir("foo");
    mkdir("bar");
    mkfile("bar/test.txt", "");
    cd("bar");
    rm("test.txt");
    cd("..");
    ls();
    return 0;
}
