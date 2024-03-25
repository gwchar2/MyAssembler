myAssembler: mainAssembler.o 
	gcc -ansi -Wall -g -pedantic mainAssembler.o -o myAssembler 
mainAssmebler.o: mainAssembler.c assembler.h 
	gcc -ansi -Wall -c -pedantic mainAssembler.c -o mainAssembler.o
clean:
	rm *.o output
