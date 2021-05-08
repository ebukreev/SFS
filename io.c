#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"

FILE *in = NULL;
FILE *out = NULL;

void execute(const char *args[3]) {
    if (strcmp(args[0], "init") == 0) {
        init_fs();
    } else if (strcmp(args[0], "pwd") == 0) {
        if (out != NULL) {
            fprintf(out, "%s\n", pwd());
        } else {
            fprintf(stdout, "%s\n", pwd());
        }
    } else if (strcmp(args[0], "ls") == 0) {
        if (out != NULL) {
            fprintf(out, "%s", ls());
        } else {
            fprintf(stdout, "%s", ls());
        }
    } else if (strcmp(args[0], "cd") == 0) {
        cd(args[1]);
    } else if (strcmp(args[0], "mkdir") == 0) {
        mkdir(args[1]);
    } else if (strcmp(args[0], "mkfile") == 0) {
        mkfile(args[1], args[2]);
    } else if (strcmp(args[0], "show") == 0) {
        if (out != NULL) {
            fprintf(out, "%s\n", show_content(args[1]));
        } else {
            fprintf(stdout, "%s\n", show_content(args[1]));
        }
    } else if (strcmp(args[0], "rm") == 0) {
        rm(args[1]);
    } else if (strcmp(args[0], "exit") == 0) {
        if (in != NULL && out != NULL) {
            fclose(in);
            fclose(out);
        }
        exit(0);
    }
}

char *reallocating_fgets(char **bufp, size_t *sizep, FILE *fp) {
    size_t len = strlen(*bufp);

    if (fgets(*bufp, *sizep, fp) == NULL)
        return NULL;

    while (strchr(*bufp, '\n') == NULL) {
        *sizep += 100;
        *bufp = realloc(*bufp, *sizep);
        if(fgets(*bufp + len, *sizep - len, fp) == NULL)
            return *bufp;
        len += strlen(*bufp + len);
    }

    return *bufp;
}

void read_and_execute() {
    size_t len = 32;
    char *line = malloc(len);
    if (in != NULL) {
        reallocating_fgets(&line, &len, in);
    } else {
        reallocating_fgets(&line, &len, stdin);
    }

    line[strlen(line) - 1] = '\0';

    char **result = calloc(3, sizeof (*result));
    for (int i =0; i < 3; i++) {
        result[i] = calloc(len, sizeof (char));
    }

    int res_index = 0;
    int index = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ' && res_index != 2) {
            res_index++;
            index = 0;
        } else {
            result[res_index][index] = line[i];
            index++;
        }
    }

    execute((const char **) result);
    free(line);
}

int main(int argc, char *argv[]) {
    if (argc == 4 && strcmp(argv[1], "-f") == 0) {
        in = fopen(argv[2], "r");
        out = fopen(argv[3], "w");
    }
    for (;;) {
        read_and_execute();
    }
}

