assembler: main.o macro.o label_utils.o memory_utils.o parser.o syntax_check.o linked_list.o
	gcc -ansi -Wall -pedantic main.o macro.o label_utils.o memory_utils.o parser.o syntax_check.o linked_list.o -o assembler

main.o: main.c macro.h label_utils.h memory_utils.h parser.h syntax_check.h linked_list.h def.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

macro.o: macro.c macro.h parser.h syntax_check.h def.h
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o

label_utils.o: label_utils.c macro.h label_utils.h memory_utils.h parser.h syntax_check.h linked_list.h def.h
	gcc -c -ansi -Wall -pedantic label_utils.c -o label_utils.o

memory_utils.o: memory_utils.c memory_utils.h label_utils.h syntax_check.h linked_list.h def.h
	gcc -c -ansi -Wall -pedantic memory_utils.c -o memory_utils.o

parser.o: parser.c parser.h def.h
	gcc -c -ansi -Wall -pedantic parser.c -o parser.o

syntax_check.o: syntax_check.c syntax_check.h def.h
	gcc -c -ansi -Wall -pedantic syntax_check.c -o syntax_check.o

linked_list.o: linked_list.c linked_list.h def.h
	gcc -c -ansi -Wall -pedantic linked_list.c -o linked_list.o

