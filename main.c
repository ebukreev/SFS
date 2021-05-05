#include <stdio.h>
#include "fs.h"

int main() {
    init_fs();
    mkdir("foo");
    mkdir("bar");
    mkdir("baz");
    mkdir("bax");
    mkdir("bac");
    ls();
    return 0;
}
