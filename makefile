myAssembler: mainAssembler.o funcs.o 
	gcc -ansi -Wall -g -pedantic mainAssembler.o funcs.o -o myAssembler 
mainAssmebler.o: mainAssembler.c assembler.h 
	gcc -ansi -Wall -c -pedantic mainAssembler.c -o mainAssembler.o
funcs.o: funcs.c assembler.h
	gcc -ansi -Wall -c -pedantic funcs.c -o funcs.o	
clean:
	rm *.o output
