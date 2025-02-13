#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

int main (int argc, char ** argv)
{
    pid_t pid = getpid();
    extern char** environ;

    int status;
    char buf[MAX_BUFFER];                       // line buffer
    char * args[MAX_ARGS];                      // pointers to arg strings
    char ** arg;			                    // working pointer thru args
    char *path_buf;                             // path buffer
    long size;                                  // path size variable
    char *path;                                 // stores path
    char *prompt;                               // stores prompt (derivative of path)
    size = pathconf(".", _PC_PATH_MAX);
    prompt = malloc((size_t)size);
    if ((path_buf = (char*)malloc((size_t)size)) != NULL) 
    	path = getcwd(path_buf, (size_t)size); // get current path
    strcpy(prompt, path);
    strcat(prompt, ": ");
    /* keep reading input until "quit" command or eof of redirected input */
    while (!feof(stdin)) { 
        /* get command line from input */
        fputs (prompt, stdout); // write prompt
        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input

            while ((*arg++ = strtok(NULL,SEPARATORS)));
            // last entry will be NULL 

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

                        else{
                            printf("Unknown command\n");
                            exit(0);
                        }
                        exit(0); // exits process after executing command(the program does something very bad without this exit)
                    default:
                        waitpid(0, &status, WUNTRACED);	
                    }
                

                /* else pass command onto OS (or in this instance, print them out) */
                arg = args;
                // while (*arg) {
                //     fprintf(stdout,"%s ",*arg++);
                //     fputs ("\n", stdout);
                // }
            }
        }
    }
    return 0;
}

