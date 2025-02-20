/* 

strtokeg - skeleton shell using strtok to parse command line
usage:
strtokeg
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define SEPARATORS).
can use redirected input if the first token is a recognised internal command, 
then that command is executed. otherwise the tokens are printed on the display.

internal commands:
clear - clears the screen
quit - exits from the program
********************************************************************
version: 1.0
date:    December 2003
author:  Ian G Graham
School of Information Technology
Griffith University, Gold Coast
ian.graham@griffith.edu.au
copyright (c) Ian G Graham, 2003. All rights reserved.
This code can be used for teaching purposes, but no warranty,
explicit or implicit, is provided.
*******************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <./parse.h> // custom header file, contains a function that parses strings.

#define SEPARATORS " \t\n"                     // token separators

int main (int argc, char ** argv)
{
    pid_t pid = getpid();
    extern char** environ;

    int status;                                 // stores process status
    char ** args;                               // pointers to arg strings
    int arg_count;                              // stores amount of arguments
    char buf[1024];                             // initialise buffer storage with maximum input size

    char *path_buf;                             // path buffer
    long size;                                  // path size variable
    char *path;                                 // stores path
    char *prompt;                               // stores prompt (derivative of path)
    char *envr;                                 // environment storage. used to write shell path into environ variable

    size = pathconf(".", _PC_PATH_MAX); // get maximum path length from the system
    envr = malloc((size_t)size); // store environment
    prompt = malloc((size_t)size); // store prompt
    if ((path_buf = (char*)malloc((size_t)size)) != NULL){
        path = getcwd(path_buf, (size_t)size); // get current path
        strcpy(prompt, path); // copy path into prompt
        strcat(prompt, ": "); // append a semicolon (or any other prompt form)
    
        /*following 3 lines create a new environment variable and put it into the environ*/

        strcpy(envr, "shell="); // create the start of the line
        strcat(envr, path); // concatenate the start and the path
        putenv(envr); // put it in the variable list
    }
    
    
    /* keep reading input until "quit" command or eof of redirected input */
    while (!feof(stdin)) { 
	    /* get command line from input */
        fputs (prompt, stdout); // write prompt
        if (fgets (buf, 1024, stdin)) { // read a line

            arg_count = parse(buf, &args); // parse the input string into an array of strings (more details in parse.h)
            
            /*
            * cd implementation.
            * cd can only be executed in the parent process, executing it
            * in child process yields no result
            */
            if (!strcmp(args[0],"cd")){
                if (args[1] == NULL){
                    ;
                }
                else{
                    chdir(args[1]);
                    path = getcwd(path_buf, (size_t)size);
                    strcpy(prompt, path);
                    strcat(prompt, ": ");
                }
            }
            
            else if (args[0]) {             // if there's anything there
                                           
                switch (fork()){            // fork to exec process
                    case -1:
                        printf("Fork didn't succeed.");
                        break;
                    case 0:
                       /* put commands here */

                        if (!strcmp(args[0],"clr")) { // "clr" command
                            system("clear");
                            exit(0);
                        }
                     
                        else if (!strcmp(args[0],"quit")) {   // "quit" command
                            kill(pid, SIGQUIT);          // end process
                            exit(0);
                        }

                        
                        else if (!strcmp(args[0],"dir")){
                            // execlp("ls", "ls", NULL, NULL);
                            execvp(args[0], args);
                        }

                        else if (!strcmp(args[0],"environ")){
                            int i = 0;
                            while(environ[i] != NULL){
                                printf("%s\n", environ[i]);
                                i++;
                            }
                        }
                        else{
                            printf("Unknown command\n");
                            exit(0);
                        }
                        exit(0); // exits process after executing command(the program does something very bad without this exit)
                    default:
                        waitpid(0, &status, WUNTRACED);
                    }
            }
        }
    }
    return 0;
}

