/*
    Ivan Fedorov
    23735465
    I acknowledge the DCU Academic Integrity Policy
*/
#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>

int parse(char* unparsed, char*** args, char* separators);
int shift_nulls(char*** args, int arg_count);
void cleanup(char*** args, int argc);

#endif