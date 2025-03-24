/*
    Ivan Fedorov
    23735465
    I acknowledge the DCU Academic Integrity Policy
*/
#ifndef IOSET_H
#define IOSET_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int setio(int arg_count, char*** args, int* output_desc, int* input_desc, bool* stdout_flag, bool* stdin_flag);
#endif