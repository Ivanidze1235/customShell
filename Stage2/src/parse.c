#include "parse.h"

int parse(char* unparsed, char*** args, char* separators){ 
    /*
    the function is passed an unparsed string and a triple char pointer.
    The thriple char pointer is used as a pointer to a two-dimensional char array, 
    which in itself represents an array of strings.
    The function returns the amount of arguments in the parsed string.
    */ 

    if (!strcmp(unparsed, ""))
    {
        return -1;
    }
    
    int mem_size = 8;
    *args = calloc(mem_size, sizeof(char*));    // we allocate memory for MAX_ARGS amount of char pointers
    if (!(*args))
    {
        printf("Could not allocate memory for parsed input.\n");
    }
    
    int argc = 0;                               // initialise our counter so we can return the amount of arguments

    char *buf;                                  // initialise buffer for storing tokens

    buf = strtok(unparsed, separators);                             // get first token
    **args = calloc(strlen(buf) + 4, sizeof(char));                 // allocate memory for first element
    
    strcpy(*args[0], buf);                                          // copy buffer to first element
    argc++;                                                         // increment counter

    while((buf = strtok(NULL, separators))){                        // repeats the above process until there is no more string left
        if (mem_size == (argc - 1))
        {
            mem_size = mem_size + 8;
            *args = realloc(*args, mem_size * sizeof(char*));       // increase array size if running out of space
            if (!(*args))
            {
                printf("Could not allocate memory for parsed input.\n");
            }
        }
        
        *(*args + argc) = calloc(strlen(buf) + 4, sizeof(char));    // allocate memory for next element
        if (!(*(*args + argc)))
        {
            printf("Could not allocate memory for parsed element.\n");
        }
        strcpy(*(*args + argc), buf);                               // copy buffer to element
        argc++;                                                     // increment counter
    }
    return argc;        // our counter represents the amount of arguments, so we return it to use in the main function
}

void cleanup(char*** args, int argc){
    /*
        This function cleans up the argument array created by the parse function.
        It goes through every element of our string array and frees that memory.
    */
    for (int i = 0; i < argc; i++)      // loop through every element
    {
        free(*(*args + i));             // free element
        *(*args + i) = NULL;            // assign NULL to the pointer
    }
    free(*args);                        // free the whole array
    *args = NULL;                       // assign NULL to the pointer
}