# Ivan Fedorov, 23735464 - Operating Systems Project 1
# csc1021/customshell

customshell: src/customshell.c src/parse.h src/getpath.h src/parse.c src/getpath.c src/ioset.h src/ioset.c
	gcc -Wall src/customshell.c src/getpath.c src/parse.c src/ioset.c -I . -o bin/customshell

.PHONY: clear run
clear:
	rm bin/customshell
run:
	bin/customshell
