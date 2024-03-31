#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"


/* defining comamnds array */
char *commands[NUM_OF_CMDS] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "not",
        "clr" ,
        "lea" ,
        "inc",
        "dec",
        "jmp",
        "bne",
        "red",
        "prn" ,
        "jsr" ,
        "rts" ,
        "hlt"
} ;    
/* defining registers array */
char *registers[NUM_OF_REGS] = {
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6" ,
        "r7" ,
        "r8",
        "r9",
} ;  
/* defining errors array */
struct Error errors[] = {
        {1,"Undefined register name\n"},
        {2, "Undefined command name\n"},
        {3, "Extraneous text after end of command\n"},
        {4, "Undefined argument\n"},
        {5, "Missing argument\n"},
        {6, "Illegal comma\n"},
        {7, "Missing comma\n"},
        {8, "Multiple consecutive commas\n"},
        {9, "Segmenataion fault\n"}
} ;    


int main(int argc,char *argv[]) {

    FILE *fp ;
    char file_name[FILE_NAME_LEN] ;
    int file_count = 0 ; /* pointer to argv index */

     /* checking arguments */
    if (argc < MIN_ARGV) /* no arguments */ {
        fprintf(stderr, "error: no arguments.\n");
        exit(1) ;
    }
    file_count++ ; /* argumants found. increase file count */
    /* opening file and checking success */
    strcpy(file_name, argv[1]) ;
    addFileEnding(file_name) ;
    fp = fopen(file_name, "r") ;
    if (fp==NULL){
        fprintf(stderr, "error: file opening failed.\n");
        exit(1) ;
    }

    else 
    printf("success\n");

    pre_assembler(fp) ;

    
    
    return 0;
}
