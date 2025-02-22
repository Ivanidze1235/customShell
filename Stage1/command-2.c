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

/*
    Shell by Ivan Fedorov
    Reads a line from input, splits it into tokens, stores the separated string in an array,
    checks if it is a known command, if it is, executes the command. 
    Heavily inspired by strtokeg program by Ian G Graham
    *******************************************************

    Date: February 2025
    Author: Ivan Fedorov
    Dublin City University
    ivan.fedorov2@mail.dcu.ie
*/

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
            if (!strcmp(args[0],"cd")){ // check if argument is cd
                if (args[1] == NULL){
                    ; // TODO: add reporting current directory
                }
                else{
                    chdir(args[1]); // changes current working directory
                    path = getcwd(path_buf, (size_t)size); // gets new path
                    strcpy(prompt, path); // set new prompt
                    strcat(prompt, ": ");
                }
            }
            
            else if (args[0]) {             // check if the first argument is not NULL
                                           
                switch (fork()){            // fork to exec process
                    case -1:
                        printf("Fork didn't succeed.");
                        break;
                    case 0:
                       /* put commands here */

                        if (!strcmp(args[0],"clr")) { // "clr" command
                            system("clear");          // makes a system call to clear the screen.
                        }
                     
                        else if (!strcmp(args[0],"quit")) {   // "quit" command
                            kill(pid, SIGQUIT);          // end parent process
                        }

                        else if (!strcmp(args[0], "echo")){ // "echo" command
                            for(int i = 1; i < arg_count; i++){ // prints every stored argument
                                printf("%s ", args[i]);
                            }
                            printf("\n"); // adds a newline in the end
                        }

                        
                        else if (!strcmp(args[0],"dir")){ // "dir" command
                            execvp("ls", args);           // executes ls with passed arguments
                        }

                        else if (!strcmp(args[0],"environ")){ // "environ" command, prints all environment variables
                            int i = 0;
                            while(environ[i] != NULL){ // loop through all elements of environ
                                printf("%s\n", environ[i]);
                                i++;
                            }
                        }
                        else{ // in case user uses an unknown command
                            printf("Unknown command\n");
                        }
                        exit(0); // exits process after executing command (the program does something very bad without this exit)
                    default:
                        waitpid(0, &status, WUNTRACED);
                    }
            }
            cleanup(&args, arg_count);
        }
        
    }
    return 0;
}

