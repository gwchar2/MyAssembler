#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/assembler.h"


char *commands[NUM_OF_CMDS] = {
    "mov", /* 0, gruop 1 */
    "cmp", /* 1, gruop 1 */
    "add", /* 2, gruop 1 */
    "sub", /* 3, gruop 1 */
    "not", /* 4, gruop 2 */
    "clr", /* 5, gruop 2 */
    "lea", /* 6, gruop 1 */
    "inc", /* 7, gruop 2 */
    "dec", /* 8, gruop 2 */
    "jmp", /* 9, gruop 2 */
    "bne", /* 10, gruop 2 */
    "red", /* 11, gruop 2 */
    "prn", /* 12, gruop 2 */
    "jsr", /* 13, gruop 2 */
    "rts", /* 14 */
    "hlt" /* 15 */
};    

char *registers[NUM_OF_REGS] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6" ,
    "r7" ,
    "PSW",
    "PC"
};

int curr_line_number = 0;
int IC = 0;
int DC = 0;
int err_flag = 0;
label_node *lbl_head = NULL;                                                        /* Label table head */
label_node *dc_head = NULL;                                                         /* Data segment head */
cmd_node *cmd_head = NULL;                                                          /* Instruction segment head */
ErrorCode errorCode = 0;                                                            /* Global error variable */


int main(int argc,char *argv[]) {

    FILE *fp ;
    char *file_name = NULL ;
    char *temp_name = NULL ; 
    char *clean_file_name = (char*)malloc(FILE_NAME_LEN) ;                          /* string to hold the name as recieved in command line. no endings. */
    int file_count = 0 ;                                                            /* pointer to argv index */
    check_allocation(clean_file_name);
    /* checking arguments */
    if (argc < MIN_ARGV)                                                            /* no arguments */ {
        fprintf(stderr, "error: no arguments.\n");
        exit(1) ;
    }
    file_count++ ; /* argumants found. increase file count */
    strcpy(clean_file_name, argv[1]) ; /* getting file name */
    /* adding the .as ending */
    temp_name = addFileEnding(clean_file_name,0) ;
    file_name = (char*)malloc(strlen(temp_name)+1) ;/* allocate memory for file name with ending */
    check_allocation(file_name);
    strcpy(file_name,temp_name) ; /* add .as ending to filename */
    fp = fopen(file_name, "r") ;
    check_allocation(fp);

    preAssembler(fp,clean_file_name) ;

    fclose(fp);
    
    /* adding the .am ending */
    temp_name = addFileEnding(clean_file_name,1) ;
    file_name = (char*)malloc(strlen(temp_name)+1) ;/* allocate memory for file name with ending */
    check_allocation(file_name);
    strcpy(file_name,temp_name) ; /* add .as ending to filename */
    fp = fopen(file_name, "r") ;
    check_allocation(fp);
    
    scan_file(fp);
    printf("Hello?");
    printList(1);
    return 0;
}
