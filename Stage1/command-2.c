/*
    Ivan Fedorov
    23735465
    I acknowledge the DCU Academic Integrity Policy
*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#include <./parse.h> // custom header file, contains a function that parses strings.

#define SEPARATORS " \t\n"                     // token separators

int main (int argc, char ** argv)
{
    pid_t pid = getpid();                       // gets and stores parent process PID
    extern char** environ;                      // environment variables list

    int status;                                 // stores process status
    char ** args;                               // pointers to arg strings
    int arg_count;                              // stores amount of arguments
    char buf[1024];                             // initialise buffer storage with maximum input size

    char *path_buf;                             // path buffer
    long size;                                  // path size variable
    char *path;                                 // stores path
    char *prompt;                               // stores prompt (derivative of path)
    char *envr;                                 // environment storage. used to write shell path into environ variable
    char *help_path;                            // path to helpfile

    bool bg_exec = 0;
    bool batchfile = 0;                         // boolean that checks whether a batchfile is used
    FILE *fptr;                                 // pointer to store opened file (will probably rename to something more specific)

    size = pathconf(".", _PC_PATH_MAX);         // get maximum path length from the system
    envr = malloc((size_t)size);                // store environment
    help_path = malloc((size_t)size);                // store helpfile directory
    prompt = malloc((size_t)size);              // store prompt
    if ((path_buf = (char*)malloc((size_t)size)) != NULL){
        path = getcwd(path_buf, (size_t)size);  // get current path
        strcpy(prompt, path);                   // copy path into prompt
        strcat(prompt, ": ");                   // append a semicolon (or any other prompt form)
    
        /*following 3 lines create a new environment variable and put it into the environ*/

        strcpy(envr, "SHELL=");                 // create the start of the line
        strcat(envr, path);                     // concatenate the start and the path
        putenv(envr);                           // put it in the variable list

        strcpy(help_path, path);                 // copy path to help_path
        strcat(help_path, "/help.txt");          // add filename
    }

    if (argv[1]){
        if ((fptr = freopen(argv[1], "r", stdin)) == NULL){
            printf("Could not open file!");
        }
        else{
            batchfile = 1;
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
            if (buf[0] != 10){                              // check if input is newline
                arg_count = parse(buf, &args);              // parse the input string into an array of strings (more details in parse.h)
            }
            else{
                continue;                                   // skip to next input
            }
            
            bg_exec = !strcmp(args[arg_count - 1], "&");    // set background execution to true or false
            /*
            * cd implementation.
            * cd can only be executed in the parent process, executing it
            * in child process yields no result
            */
            if (!strcmp(args[0],"cd")){                     // check if argument is cd
                if (args[1] == NULL){
                    printf("%s", path);
                    printf("\n");
                }
                else{
                    if(!chdir(args[1])){                                // tries to change current working directory
                        path = getcwd(path_buf, (size_t)size);          // gets new path
                        strcpy(prompt, path);                           // set new prompt
                        strcat(prompt, ": ");
                    }
                    else{
                        printf("directory does not exist.\n");
                    }
                }
            }
            
            else if (args[0]) {                 // check if the first argument is not NULL
                                           
                switch (fork()){                // fork to exec process
                    case -1:
                        printf("Fork didn't succeed.");
                        break;
                    case 0:
                       /* put commands here */

                        if (!strcmp(args[0],"clr")) {               // "clr" command
                            system("clear");                        // makes a system call to clear the screen.
                        }
                     
                        else if (!strcmp(args[0],"quit")) {         // "quit" command
                            kill(pid, SIGQUIT);                     // end parent process
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
                        }
                        
                        else if (!strcmp(args[0],"dir")){           // "dir" command
                            execvp("ls", args);                     // executes ls with passed arguments
                        }

                        else if (!strcmp(args[0],"environ")){       // "environ" command, prints all environment variables
                            int i = 0;
                            while(environ[i] != NULL){              // loop through all elements of environ
                                printf("%s\n", environ[i]);
                                i++;
                            }
                        }

                        else if (!strcmp(args[0],"help")){                      // "help" command
                            
                            FILE *helpfile = fopen(help_path, "r");             // opens the file containing helpful info
                            if (helpfile == NULL)                               // check if the file was opened successfully
                            {
                                printf("Couldn't open help file.\n");           // report error otherwise
                            }
                            else
                            {
                                char temp[1024];                                // create temp storage for strings
                                while (fgets(temp, 1024, helpfile) != NULL) {   // loop through lines in file
                                    printf("%s", temp);                         // print line
                                }
                                printf("\n");
                                fclose(helpfile);                               // close file after reading it
                            }
                            
                        }

                        else{                           // in case user uses an external command
                            execvp(args[0], args);      // execute external command
                        }
                        exit(0);                        // exits process after executing command (the program does something very bad without this exit)
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

