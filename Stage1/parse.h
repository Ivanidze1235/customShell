#define BUF_SIZE 256
#define MAX_ARGS 32

int parse(char* unparsed, char*** args){
    *args = calloc(MAX_ARGS, sizeof(char*));
    int argc = 0;

    char *buf;

    buf = strtok(unparsed, " \t\n");
    **args = calloc(strlen(buf) + 4, sizeof(char));
    strcpy(*args[0], buf);
    argc++;

    while(buf = strtok(NULL, " \t\n")){
        *(*args + argc) = calloc(strlen(buf) + 4, sizeof(char));
        strcpy(*(*args + argc), buf);
        argc++;
    }
    return argc;
}