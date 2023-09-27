assembler: assembler.o error_handler.o  lexical_analysis.o syntax_analysis.o first.o  variables.o labelHandler.o commandHandler.o utility.o dataHandler.o seconde.o
	gcc -ansi -pedantic -Wall -o assembler assembler.o error_handler.o lexical_analysis.o syntax_analysis.o first.o variables.o labelHandler.o commandHandler.o utility.o dataHandler.o seconde.o

assembler.o: assembler.c assembler.h
	gcc -ansi -pedantic -Wall -c assembler.c

first.o: first.c first.h
	gcc -ansi -pedantic -Wall -c first.c

seconde.o: seconde.c seconde.h
	gcc -ansi -pedantic -Wall -c seconde.c

labelHandler.o: labelHandler.c labelHandler.h
	gcc -ansi -pedantic -Wall -c labelHandler.c

error_handler.o: error_handler.c error_handler.h
	gcc -ansi -pedantic -Wall -c error_handler.c

lexical_analysis.o: lexical_analysis.c lexical_analysis.h 
	gcc -ansi -pedantic -Wall -c lexical_analysis.c

syntax_analysis.o: syntax_analysis.c syntax_analysis.h
	gcc -ansi -pedantic -Wall -c syntax_analysis.c

variables.o: variables.c variables.h
	gcc -ansi -pedantic -Wall -c variables.c

utility.o: utility.c utility.h
	gcc -ansi -pedantic -Wall -c utility.c

commandHandler.o: commandHandler.c commandHandler.h
	gcc -ansi -pedantic -Wall -c commandHandler.c

dataHandler.o: dataHandler.c dataHandler.h
	gcc -ansi -pedantic -Wall -c dataHandler.c

