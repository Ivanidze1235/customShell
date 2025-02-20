#define MAX_ARGS 32 // maximum amount of arguments (potentially replace these static numbers with dynamic mem allocation)

int parse(char* unparsed, char*** args){ /*
    the function is passed an unparsed string and atriple char pointer.
    The thriple char pointer is used as a pointer to a two-dimensional char array, which in itself represents an array of strings
    */ 
    *args = calloc(MAX_ARGS, sizeof(char*)); // we allocate memory for MAX_ARGS amount of char pointers
    int argc = 0; // initialise our counter so we can return the amount of arguments

    char *buf; // initialise buffer for storing tokens

    buf = strtok(unparsed, " \t\n"); // get first token
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