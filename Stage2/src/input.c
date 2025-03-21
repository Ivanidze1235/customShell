#include "input.h"

void read_input(char** input){
    /*
    This function reads raw input string using fgetc() - character by character.
    It allocates memory for the string in blocks of 64 characters.
    Input has to be freed inside main function after parsing.
    */
    char buf;                                           // storage for current character
    int size_input = 64;                                // chunk size
    *input = calloc(size_input, sizeof(char));          // allocating first chunk of memory
    if (!(*input))
    {
        printf("Could not read input.\n");
    }
    
    while((buf = getc(stdin)) != EOF && buf != '\n'){                         // get characters until newline found
        int str_len = strlen(*input);
        *(*input + str_len) = buf;
        *(*input + str_len + 1) = '\0';          // append new character
        if (str_len == (size_input - 1))         // check if running out of allocated memory
        {
            size_input += 64;
            *input = realloc(*input, size_input);       // reallocate to allow for 64 more characters
        }
    }
}