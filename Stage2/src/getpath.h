#ifndef GETPATH_H
#define GETPATH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

void getpath(char** argv, size_t path_size, char** path_bin);

#endif