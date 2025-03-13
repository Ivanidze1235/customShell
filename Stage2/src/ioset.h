#ifndef IOSET_H
#define IOSET_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int setio(int arg_count, char*** args, int* output_desc, int* input_desc);
#endif