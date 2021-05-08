#include <stdio.h>
#include <string.h>
#include "fs.h"

static void test_fs_init() {
    printf("run fs init test\n");
    init_fs();
    bool ok = true;
    if (current_node != root_node) {
        printf("fs is not created\n");
        ok = false;
    }
    if (current_node->children_count != 0) {
        printf("root children is not empty\n");
        ok = false;
    }
    if (sizeof(Node) * ARRAY_START_SIZE != sizeof(Node) * current_node->children_size) {
        printf("wrong amount of memory allocated\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_mkdir() {
    printf("run mkdir test\n");
    init_fs();
    bool ok = true;
    mkdir("foo");
    mkdir("bar");
    mkdir("baz");
    if (current_node->children_count != 3) {
        printf("children count != 3\n");
        ok = false;
    }
    if (strcmp(current_node->children[0].name, "foo") != 0) {
        printf("children[0] !is foo\n");
        ok = false;
    }
    if (strcmp(current_node->children[1].name, "bar") != 0) {
        printf("children[1] !is bar\n");
        ok = false;
    }
    if (strcmp(current_node->children[2].name, "baz") != 0) {
        printf("children[2] !is baz\n");
        ok = false;
    }
    if (current_node->children[0].parent != current_node) {
        printf("children parent != current node\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_mkfile() {
    printf("run mkfile test\n");
    init_fs();
    bool ok = true;
    mkfile("foo", "foo");
    mkfile("bar", "");
    mkfile("baz", "fffffffff");
    if (current_node->children_count != 3) {
        printf("children count != 3\n");
        ok = false;
    }
    if (strcmp(current_node->children[0].name, "foo") != 0) {
        printf("children[0] !is foo\n");
        ok = false;
    }
    if (strcmp(current_node->children[1].name, "bar") != 0) {
        printf("children[1] !is bar\n");
        ok = false;
    }
    if (strcmp(current_node->children[2].name, "baz") != 0) {
        printf("children[2] !is baz\n");
        ok = false;
    }
    if (current_node->children[0].parent != current_node) {
        printf("children parent != current node\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_show_content() {
    printf("run show_content test\n");
    init_fs();
    bool ok = true;
    mkfile("foo", "foobarbaz");
    if (strcmp(show_content("foo"), "foobarbaz") != 0) {
        printf("different content\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_cd() {
    printf("run cd test\n");
    init_fs();
    bool ok = true;
    mkdir("foo");
    cd("foo");
    if (strcmp(current_node->name, "foo") != 0) {
        printf("current node not is foo\n");
        ok = false;
    }
    mkdir("bar");
    cd("../");
    if (strcmp(current_node->name, "/") != 0) {
        printf("current node not is root\n");
        ok = false;
    }
    cd("/foo/bar");
    if (strcmp(current_node->name, "bar") != 0) {
        printf("current node not is bar\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_pwd() {
    printf("run pwd test\n");
    init_fs();
    bool ok = true;
    mkdir("foo");
    cd("foo");
    if (strcmp(pwd(), "/foo/") != 0) {
        printf("/foo/ expect\n");
        ok = false;
    }
    mkdir("bar");
    cd("../");
    if (strcmp(pwd(), "/") != 0) {
        printf("/ expect\n");
        ok = false;
    }
    cd("/foo/bar");
    if (strcmp(pwd(), "/foo/bar/") != 0) {
        printf("/foo/bar/ expect\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_ls() {
    printf("run ls test\n");
    init_fs();
    bool ok = true;
    mkdir("foo");
    mkdir("bar");
    mkdir("baz");
    if (strcmp(ls(), "foo\nbar\nbaz\n") != 0) {
        printf("foo\nbar\nbaz\n expect\n");
        ok = false;
    }
    cd("bar");
    mkfile("/bar/test.txt", "");
    if (strcmp(ls(), "test.txt\n") != 0) {
        printf("test.txt\n expect\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

static void test_rm() {
    printf("run rm test\n");
    init_fs();
    bool ok = true;
    mkdir("foo");
    mkdir("bar");
    mkdir("bar/boo");
    rm("bar");
    if (strcmp(ls(), "foo\n") != 0) {
        printf("bar not deleted\n");
        ok = false;
    }
    if (current_node->children_count != 1) {
        printf("children count != 1\n");
        ok = false;
    }
    rm("/");
    if (ok) {
        printf("all ok\n");
    }
}

int main() {
    test_fs_init();
    test_mkdir();
    test_mkfile();
    test_show_content();
    test_cd();
    test_pwd();
    test_ls();
    test_rm();
    return 0;
}