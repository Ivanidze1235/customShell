# Ivan Fedorov, 23735464 - Operating Systems Project 1
# I acknowledge the DCU Academic Integrity Policy
# csc1021/customshell

customshell: src/*
	gcc -Wall src/*.c -I . -o bin/customshell

.PHONY: clear run
clear:
	rm bin/customshell
run:
	bin/customshell
