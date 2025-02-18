#define BUF_SIZE 256
#define MAX_ARGS 32

void parse(char* unparsed, char*** args, int* argc){
    *args = calloc(MAX_ARGS, sizeof(char*));
    *argc = 0;

    char *buf;

    buf = strtok(unparsed, " \t\n");

    *args[0] = calloc(strlen(buf) + 4, sizeof(char));
    strcpy(*args[0], buf);
    //printf("This is our stored value: %s\n", *args[0]);
    *argc++;
    while(buf = strtok(NULL, " \t\n")){ // somewhere in this loop a bad thing happens
        //printf("buffer: %s\n", buf);
        *args[*argc] = calloc(strlen(buf) + 4, sizeof(char));
        //printf("this print stops the program from segfaulting for some reason \n");
        strcpy(*args[*argc], buf);
        //printf("number: %d\n", *argc);
        //printf("value: %s\n", *args[*argc]);
        *argc++;
    }
}