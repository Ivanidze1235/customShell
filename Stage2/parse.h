#ifndef PARSE_H
#define PARSE_H

int parse(char* unparsed, char*** args, char* separators);
void cleanup(char*** args, int argc);

#endif