#define BUF_SIZE 256
#define MAX_ARGS 32

void parse(char* unparsed, char** args, int* argc){
    args = calloc(MAX_ARGS, sizeof(char*));
    *argc = 0;

    char *buf;

    buf = strtok(unparsed, " ");
    //printf("This is our buf: %s\n", buf);
    args[0] = calloc(strlen(buf), sizeof(char));
    strcpy(args[0], buf);
    //printf("This is our stored value: %s\n", args[0]);
    *argc++;
    while(buf = strtok(NULL, " ")){
        args[*argc] = calloc(strlen(buf), sizeof(char));
        strcpy(args[*argc], buf);
        //printf("This is our enement %d: %s\n", argc, args[*argc]);
        *argc++;
    }
}