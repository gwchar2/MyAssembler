#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"

    /* defining registers array */
    char *registers[NUM_OF_REGISTERS] = {
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"
    }

    char *saved_words[NUM_OF_SAVED_WORDS] = {
        "entry",
        "extern",
        "define",
        "data",
        "string",
        ".entry",
        ".extern",
        ".define",
        ".data",
        ".string"
    }


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
        "prn",
        "jsr",
        "rts",
        "hlt"
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
        {9, "Segmenataion fault\n"},
        {10, "Redefenition of macro name\n"}
    } ;    



int main(int argc,char *argv[]) {

    FILE *fp ;
    char *file_name = NULL ;
    char *temp_name = NULL ; 
    char *clean_file_name = (char*)malloc(FILE_NAME_LEN) ; /* string to hold the name as recieved in command line. no endings. */
    int file_count = 0 ; /* pointer to argv index */


    if (clean_file_name == NULL) {
        fprintf(stderr, "error: memory allocation failed.\n");
        exit(1) ;    
    }

     /* checking arguments */
    if (argc < MIN_ARGV) /* no arguments */ {
        fprintf(stderr, "error: no arguments.\n");
        exit(1) ;
    }
    file_count++ ; /* argumants found. increase file count */

    /* getting file name */
    strcpy(clean_file_name, argv[1]) ;

    /* adding the .as ending */
    temp_name = addFileEnding(clean_file_name,0) ;
    file_name = (char*)malloc(strlen(temp_name)+1) ;/* allocate memory for file name with ending */
    if (file_name == NULL) {
        fprintf(stderr, "error: memory allocation failed.\n");
        exit(1) ; 
    }
    strcpy(file_name,temp_name) ; /* add .as ending to filename */



    fp = fopen(file_name, "r") ;
    if (fp==NULL){
        fprintf(stderr, "error: file opening failed.\n");
        exit(1) ;
    }

    preAssembler(fp,clean_file_name) ;
    
    fclose(fp);
    return 0;
}
