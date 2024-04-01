#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"

int curr_line_number = 0;
int IC = 0;
int DC = 0;
int err_flag = 0;
label_node *lbl_head = NULL; /* Label table head */
label_node *dc_head = NULL; /* Data segment head */
cmd_node *cmd_head = NULL; /* Instruction segment head */
ErrorCode errorCode = 0; /* Global error variable */



int main(int argc,char *argv[]) {

    FILE *fp ;
    char *file_name = NULL ;
    char *temp_name = NULL ; 
    char *clean_file_name = (char*)malloc(FILE_NAME_LEN) ; /* string to hold the name as recieved in command line. no endings. */
    int file_count = 0 ; /* pointer to argv index */
    check_allocation(clean_file_name);
     /* checking arguments */
    if (argc < MIN_ARGV) /* no arguments */ {
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
    return 0;
}
