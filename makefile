myAssembler: mainAssembler.o funcs.o preAssembler.o
	gcc -ansi -Wall -g -pedantic mainAssembler.o funcs.o preAssembler.o -o myAssembler 
mainAssmebler.o: mainAssembler.c assembler.h 
	gcc -ansi -Wall -c -pedantic mainAssembler.c -o mainAssembler.o
funcs.o: funcs.c assembler.h
	gcc -ansi -Wall -c -pedantic funcs.c -o funcs.o	
preAssembler.o: preAssembler.c assembler.h
	gcc -ansi -Wall -c -pedantic preAssembler.c -o preAssembler.o	
clean:
	rm -rf build/*.o MainAssembler
