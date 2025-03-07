# Ivan Fedorov, 23735464 - Operating Systems Project 1
# csc1021/customshell

customshell: command-2.c parse.h getpath.h parse.c getpath.c
	gcc -Wall command-2.c getpath.c parse.c -I . -o bin/customshell

.PHONY: clear run
clear:
	rm bin/customshell
run:
	bin/customshell
