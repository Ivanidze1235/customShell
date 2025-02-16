#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define MAX_ARGS 32

int main(){
    char unparsed[] = "hello my name is Ivan";
    char *buf;
    char** args = calloc(MAX_ARGS, sizeof(char*));
    int argc = 0;
    buf = strtok(unparsed, " ");
    printf("This is our buf: %s\n", buf);
    args[0] = calloc(strlen(buf), sizeof(char));
    strcpy(args[0], buf);
    printf("This is our stored value: %s\n", args[0]);
    argc++;
    while(buf = strtok(NULL, " ")){
        args[argc] = calloc(strlen(buf), sizeof(char));
        strcpy(args[argc], buf);
        printf("This is our enement %d: %s\n", argc, args[argc]);
        argc++;
    }

    for(int i = 0; i < argc; i++){
        printf("%s\n", args[i]);
    }

    return 0;
}