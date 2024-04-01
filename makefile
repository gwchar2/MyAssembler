myAssembler: mainAssembler.o funcs.o preAssembler.o label_nodes.o row_nodes.o error_handler.o breakdown.o
	gcc -ansi -Wall -g -pedantic mainAssembler.o funcs.o preAssembler.o label_nodes.o row_nodes.o error_handler.o breakdown.o -o myAssembler 

mainAssmebler.o: mainAssembler.c assembler.h 
	gcc -ansi -Wall -c -pedantic mainAssembler.c -o mainAssembler.o

funcs.o: funcs.c assembler.h
	gcc -ansi -Wall -c -pedantic funcs.c -o funcs.o	

preAssembler.o: preAssembler.c assembler.h
	gcc -ansi -Wall -c -pedantic preAssembler.c -o preAssembler.o	

label_nodes.o: label_nodes.c assembler.h
	gcc -ansi -Wall -pedantic -c label_nodes.c -o label_nodes.o

row_nodes.o: row_nodes.c assembler.h
	gcc -ansi -Wall -pedantic -c row_nodes.c -o row_nodes.o

error_handler.o: error_handler.c assembler.h
	gcc -ansi -Wall -pedantic -c error_handler.c -o error_handler.o

breakdown.o: breakdown.c assembler.h
	gcc -ansi -Wall -pedantic -c breakdown.c -o breakdown.o

clean:
	rm *.o output
