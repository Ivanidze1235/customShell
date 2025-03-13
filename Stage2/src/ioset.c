#include "parse.h"
/*
    This function is responsible for redirecting input and output of a single command
*/
bool setio(int arg_count, char*** args, int* output_desc, int* input_desc)
{
    bool err = 0;                       // boolean that will be cheched after the loop to see if a file-related error has occured
    for (int i = 0; i < arg_count; i++) // look for I/O redirection loop
    {
        if (*(*args + i) != NULL) // skip if argument is NULL
        {
            if (!strcmp(*(*args + i), ">") && i < (arg_count - 1))
            {                   // output redirection (truncation)
                fflush(stdout); // flush unwritten data (just in case)
                *output_desc = open(*(*args + i + 1),
                                   O_WRONLY | O_CREAT | O_TRUNC, 0600); // open file as a descriptor (last argument is permission rights)

                if (*output_desc == -1) // check if file was successfully opened
                {
                    printf("Couldn't create file!\n");
                    err = 1;
                    break;
                }

                dup2(*output_desc, STDOUT_FILENO); // set stdout to this file

                *(*args + i) = NULL; // nullify arguments that are redirection related
                *(*args + i + 1) = NULL;
                continue;
            }

            else if (!strcmp(*(*args + i), ">>") && i < (arg_count - 1))
            { // output redirection (appending)
                fflush(stdout);
                *output_desc = open(*(*args + i + 1),
                                   O_WRONLY | O_CREAT | O_APPEND, 0600); // open file as a descriptor (with O_APPEND as parameter)

                if (*output_desc == -1) // check if file was successfully opened
                {
                    printf("Couldn't create file!\n");
                    err = 1;
                    break;
                }
                dup2(*output_desc, STDOUT_FILENO); // set stdout to this file

                *(*args + i) = NULL; // nullify arguments that are redirection related
                *(*args + i + 1) = NULL;
                continue;
            }

            else if (!strcmp(*(*args + i), "<") && i < (arg_count - 1))
            { // input redirection
                fflush(stdin);
                *input_desc = open(*(*args + i + 1), O_RDONLY); // open file as a descriptor
                if (*input_desc == -1)                     // check if file was successfully opened
                {
                    printf("File not found!\n");
                    err = 1;
                    break;
                }

                dup2(*input_desc, STDIN_FILENO); // set stdin to this file

                *(*args + i) = NULL; // nullify arguments that are redirection related
                *(*args + i + 1) = NULL;
                continue;
            }
        }
    }
    return err;
}