/*
    Ivan Fedorov
    23735465
    I acknowledge the DCU Academic Integrity Policy
*/

#include "parse.h"    // custom file, contains a function that parses strings.
#include "getpath.h"  // another custom file, contains a function that gets bin path
#include "ioset.h"    // file with function that redirects I/O
#include "input.h"    // file with input reading function

int main (int argc, char ** argv)
{
    //pid_t pid = getpid();                     // gets and stores parent process PID
    extern char** environ;                      // environment variables list

    int status;                                 // stores process status
    char ** args;                               // pointers to arg strings
    int arg_count;                              // stores amount of arguments
    char *input;                                // unparsed, dynamically allocated input

    long path_size;                             // path size variable (stores maximum path)
    char *path;                                 // temporary storage for the path

    char *path_bin;                             // stores path
    char *prompt;                               // stores prompt (derivative of path)
    char *envr;                                 // environment storage. used to write shell path into environ variable
    char *help_path;                            // path to helpfile

    bool bg_exec = 0;
    bool batchfile = 0;                         // boolean that checks whether a batchfile is used
    FILE *batchfile_ptr;                        // pointer to store opened file (will probably rename to something more specific)

    int stdout_desc = dup(STDOUT_FILENO);       // default stdout, return to this after execution
    int stdin_desc = dup(STDIN_FILENO);         // default stdin, return to this after execution

    bool stdout_flag = false;
    bool stdin_flag = false;

    path_size = pathconf(".", _PC_PATH_MAX);        // get maximum path length from the system
    envr = malloc((size_t)path_size);               // store environment
    help_path = malloc((size_t)path_size);          // store helpfile directory
    prompt = malloc((size_t)path_size);             // store prompt
    if ((path_bin = (char*)malloc((size_t)path_size)) != NULL){
        path = (char*)malloc((size_t)path_size);
        path = getcwd(path, (size_t)path_size);     // get current path

        getpath(&argv[0], path_size, &path_bin);    // function that gets the

        strcpy(envr, "");                           // create the start of the line
        strcat(envr, path_bin);                     // concatenate the start and the path
        strcat(envr, "/customshell");               // add file name
        setenv("SHELL", envr, 1);                   // put it in the variable list
        free(envr);                                 // free the variable as it is not needed anymore

        /*
        The following piece of code is an elaborate way to store help file path
        */
        strcpy(help_path, "more ");             // add "more" command to help path
        chdir(path_bin);
        chdir("..");
        char *path_temp;                        // temporary storage for the path
        path_temp = malloc((size_t)path_size);
        path_temp = getcwd(path_temp, (size_t)path_size);
        strcat(help_path, path_temp);
        free(path_temp);
        chdir(path);
        strcat(help_path, "/manual/help.txt");  // add filename

        if(chdir(path)){                        // attempt to go back to current directory
            printf("couldn't go back\n");
        }

        strcpy(prompt, path);                   // copy path into prompt
        strcat(prompt, ": ");                   // append a semicolon (or any other prompt form)
    }

    if (argv[1]){                                                       // check if there is a batchfile being input
        if ((batchfile_ptr = freopen(argv[1], "r", stdin)) == NULL){    // switch input stream to file
            printf("Could not open file!");                             // notify if not succeeded

        }
        else{
            stdin_desc = open(argv[1], O_RDONLY);
            batchfile = 1;                                              // set batchfile switch to true
        }
    }

    while (!feof(stdin)) {

        /*
        This is the main loop, where we print the prompt (if not in batchfile mode),
        read input, parse it using a function, then check if it matches one of the internal
        commands, and executing the command.
        */
        int output_desc = -1;                               // I/O file descriptor variables, set to -1 for error checking
        int input_desc = -1;

        if (!batchfile){                                    // check if batchfile is not used
            fputs (prompt, stdout);                         // write prompt
        }
        read_input(&input);                                 // function that reads input
        {

            arg_count = parse(input, &args, " \t\n");   // parse the input string into an array of strings (more details in parse.h)
            if (arg_count == -1)                        // check if no input provided, skip to next input if so
            {   
                if (!batchfile)
                {
                    continue;
                }
                else{
                    return 0;
                }
            }

            free(input);                                // don't need unparsed input anymore so it is freed
            input = NULL;

            int IOerr =                                 // check for I/O redirection
                setio(arg_count, &args, &output_desc, &input_desc, &stdout_flag, &stdin_flag);

            if (IOerr)                                  // check if there was an error of some kind
            {
                printf("An error occured.");
                if (IOerr == 1){
                    printf(" Could not create file.\n");
                }
                else if(IOerr == 2){
                    printf(" Could not open file for reading.\n");
                }

                continue;
            }


            if(args[arg_count - 1] != NULL)
            {
                bg_exec = !strcmp(args[arg_count - 1], "&");    // set background execution to true or false
                if (bg_exec)
                {
                    args[arg_count - 1] = NULL;                 // set background execution symbol to NULL so it is not read by commands
                }
            }

            shift_nulls(&args, arg_count);                      // function that removes unnecessary NULLs in the middle of the argument array

            /*
            Commands start here
            */

            if (!strcmp(args[0],"cd")){                         // check if argument is cd
                if (args[1] == NULL){                           // check whether there is no parameter
                    printf("%s", path);                         // pront out current path
                    printf("\n");
                }
                else{
                    if(!chdir(args[1])){                                // tries to change current working directory
                        path = getcwd(path, (size_t)path_size);         // gets new path
                        strcpy(prompt, path);                           // set new prompt
                        strcat(prompt, ": ");
                    }
                    else{
                        printf("directory does not exist.\n");          // notify if directory does not exist / cannot find it
                    }
                }
            }

            else if (!strcmp(args[0],"clr")) {          // "clr" command
                system("clear");                        // makes a system call to clear the screen.
            }

            else if (!strcmp(args[0],"quit")) {         // "quit" command
                return 0;
            }

            else if (!strcmp(args[0], "echo")){         // "echo" command
                for(int i = 1; i < arg_count; i++){     // prints every stored argument
                    if (args[i] != NULL)
                    {
                        printf("%s ", args[i]);
                    }
                }
                printf("\n");                           // adds a newline in the end
            }

            else if(!strcmp(args[0], "pause")){         // "pause" command
                printf("Press ENTER to continue...");
                getchar();                              // waits for ENTER to be pressed
                continue;
            }

            else if (!strcmp(args[0],"dir")){           // "dir" command
                if (args[1])                            // check if there is an argument
                {
                    char* dir_to = calloc(strlen(args[1]) + strlen("ls -al "), sizeof(char));   // add argument to prompt
                    strcpy(dir_to, "ls -al ");
                    strcat(dir_to, args[1]);
                    if (system(dir_to))                                                         // try to list given directory
                    {
                        printf("directory \"%s\" does not exist.\n", args[1]);                  // notify user if it does not exist
                        continue;
                    }
                    free(dir_to);                                                               // free memory
                    dir_to = NULL;
                }
                else{
                    system("ls -al");
                }
            }

            else if (!strcmp(args[0],"environ")){       // "environ" command, prints all environment variables
                int i = 0;
                while(environ[i] != NULL){              // loop through all elements of environ
                    printf("%s\n", environ[i]);
                    i++;
                }
            }

            else if (!strcmp(args[0],"help")){          // "help" command
                if(system(help_path)){                  // system call the help method
                    printf("Could not read help file.\n");  // notify in case it did not succeed
                }
            }

            else if (args[0]) {                         // External command execution

                switch (fork()){                        // fork to exec process
                    case -1:                            // check for error
                        printf("Fork didn't succeed.");
                        break;
                    case 0:
                        char* parent_env = malloc((size_t)path_size);   // create temporary storage for environment variable
                        strcpy(parent_env, "");
                        strcat(parent_env, path_bin);
                        strcat(parent_env, "/customshell");             // append executable name
                        setenv("PARENT", parent_env ,1);                // put line into environment
                        free(parent_env);                               // free temporary variable
                        execvp(args[0], args);                          // execute command
                        printf("Command did not execute\n");
                        exit(0);                                        // exit in case command didn't execute
                    default:
                        break;
                }
                    if (!bg_exec)                       // check if background execution flag is down
                    {
                        waitpid(0, &status, WUNTRACED); // if it is down, wait for execution
                    }
                    else{
                        continue;                       // else, do not wait
                    }
            }
            if (stdout_flag)
            {
                dup2(stdout_desc, STDOUT_FILENO);   // restore stdout stream
                fflush(stdout);                     // flush unwritten data (just in case)
                stdout_flag = false;
            }
            if (stdin_flag)
            {
                dup2(stdin_desc, STDIN_FILENO);     // restore stdin stream
                fflush(stdin);
                stdin_flag = false;
            }
            
            cleanup(&args, arg_count);          // call to a function in parse.h that frees up memory
        }
        
    }
    if (batchfile)                              // check if batchfile present
    {
        fclose(batchfile_ptr);                  // close stream
    }

    return 0;                                   // end of program
}