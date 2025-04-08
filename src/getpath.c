/*
    Ivan Fedorov
    23735465
    I acknowledge the DCU Academic Integrity Policy
*/
#include "parse.h"

void getpath(char** argv, size_t path_size, char** path_bin){
    /*
    This is a function that retrieves the path of the executable file of the program.
    Has a side effect of staying inside that directory, so there is a need go back afterwards.
    The function gets the relative path of the executable file, removes the filename, goes to that directory
    and then gets the absolute path of that directory.
    */
    char **argv_split;
    int argv_count;

    argv_count = parse(argv[0], &argv_split, "/");      // splitting first argument into parts so the path can be retrieved
    if (argv_count == -1)
    {
        printf("Error: program name is empty");
        return;
    }
    
    char *cd_to = malloc((size_t)path_size);            // variable to store relative path
    for (int i = 0; i < (argv_count - 1); i++)          // loop that reconstructs the path
    {
        strcat(cd_to, argv_split[i]);
        strcat(cd_to, "/");
    }

    if (chdir(cd_to))                                   // try to enter directory with the executable
    {
        printf("couldn't cd to executable directory\n");
    }
    *path_bin = getcwd(*path_bin, (size_t)path_size);   // get absolute path
    cleanup(&argv_split, argv_count);                   // free parsed path
}