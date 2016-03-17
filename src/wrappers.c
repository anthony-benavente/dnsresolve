#include "wrappers.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *_malloc(size_t size) {
    void *result = malloc(size);
    if (!result) {
        perror("malloc");
        _exit(EXIT_FAILURE);
    }
    return result;
}

void _fork() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        _exit(EXIT_FAILURE);
    }
}