/*
    Shell by Ivan Fedorov
    Reads a line from input, splits it into tokens, stores the separated string in an array,
    checks if it is a known command, if it is, executes the command. 
    Heavily inspired by 'strtokeg' program by Ian G Graham
    *******************************************************

    Date: February 2025
    Author: Ivan Fedorov
    Dublin City University
    ivan.fedorov2@mail.dcu.ie
*/

#include "parse.h"    // custom file, contains a function that parses strings.
#include "getpath.h"  // another custom file, contains a function that gets bin path

int main (int argc, char ** argv)
{
    //pid_t pid = getpid();                     // gets and stores parent process PID
    extern char** environ;                      // environment variables list

    int status;                                 // stores process status
    char ** args;                               // pointers to arg strings
    int arg_count;                              // stores amount of arguments
    char buf[1024];                             // initialise buffer storage with maximum input size

    long path_size;                             // path size variable
    char *path;                                 // temporary storage for the path
    
    char *path_bin;                             // stores path
    char *prompt;                               // stores prompt (derivative of path)
    char *envr;                                 // environment storage. used to write shell path into environ variable
    char *help_path;                            // path to helpfile

    bool bg_exec = 0;
    bool batchfile = 0;                         // boolean that checks whether a batchfile is used
    FILE *fptr;                                 // pointer to store opened file (will probably rename to something more specific)

    path_size = pathconf(".", _PC_PATH_MAX);         // get maximum path length from the system
    envr = malloc((size_t)path_size);                // store environment
    help_path = malloc((size_t)path_size);           // store helpfile directory
    prompt = malloc((size_t)path_size);              // store prompt
    if ((path_bin = (char*)malloc((size_t)path_size)) != NULL){
        path = (char*)malloc((size_t)path_size);
        path = getcwd(path, (size_t)path_size);  // get current path
        
        getpath(&argv[0], path_size, &path_bin); // function that gets the

        strcpy(envr, "");                     // create the start of the line
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
        strcat(help_path, "/manual/help.txt");   // add filename

        if(chdir(path)){                        // attempt to go back to current directory
            printf("couldn't go back\n");
        }

        strcpy(prompt, path);                   // copy path into prompt
        strcat(prompt, ": ");                   // append a semicolon (or any other prompt form)
    }

    if (argv[1]){                                               // check if there is a batchfile being input
        if ((fptr = freopen(argv[1], "r", stdin)) == NULL){     // switch input stream to file
            printf("Could not open file!");                     // notify if not succeeded
        }
        else{
            batchfile = 1;                                      // set batchfile switch to true
        }
    }
    
    while (!feof(stdin)) {
        /*
        This is the main loop, where we print the prompt (if not in batchfile mode),
        read input, parse it using a function, then check if it matches one of the internal
        commands, and executing the command.
        */
        if (!batchfile){                                    // check if batchfile is not used
            fputs (prompt, stdout);                         // write prompt
        }
        
        if (fgets (buf, 1024, stdin)) {                     // read a line
            if (buf[0] != 10){                              // check if input is newline, if not, parse
                arg_count = parse(buf, &args, " \t\n");     // parse the input string into an array of strings (more details in parse.h)
            }
            else{
                continue;                                   // skip to next input if input is newline
            }
            
            bg_exec = !strcmp(args[arg_count - 1], "&");    // set background execution to true or false
            if (bg_exec)
            {
                args[arg_count - 1] = NULL;                 // set background execution symbol to NULL so it is not read by commands
            }
            

            if (!strcmp(args[0],"cd")){                     // check if argument is cd
                if (args[1] == NULL){                       // check whether there is no parameter
                    printf("%s", path);                     // pront out current path
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
                    printf("%s ", args[i]);
                }
                printf("\n");                           // adds a newline in the end
            }

            else if(!strcmp(args[0], "pause")){         // "pause" command
                printf("Press ENTER to continue...");
                getchar();                              // waits for ENTER to be pressed
                continue;
            }
            
            else if (!strcmp(args[0],"dir")){           // "dir" command
                system("ls -al");                           // executes ls with passed arguments
                //TODO: add parameter support
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

            else if (args[0]) {                 // External command execution
                                           
                switch (fork()){                // fork to exec process
                    case -1:                    // check for error
                        printf("Fork didn't succeed.");
                        break;
                    case 0:
                        char* parent_env = malloc((size_t)path_size);   // create temporary storage for environment variable
                        strcpy(parent_env, "");
                        strcat(parent_env, path_bin);
                        strcat(parent_env, "/customshell");             // append executable name
                        setenv("PARENT", parent_env ,1);                // put line into environment
                        free(parent_env);                               // free temporary variable

                        execvp(args[0], args);                          // execute command (does not work currently)
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
            cleanup(&args, arg_count);          // call to a function in parse.h that frees up memory
        }
        
    }
    if (batchfile)                              // check if batchfile present
    {
        fclose(fptr);                           // close stream
    }
    
    return 0;                                   // end of program
}

