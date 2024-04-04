#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/assembler.h"


char *commands[NUM_OF_CMDS] = {
    "mov","cmp","add","sub","not","clr",
    "lea" ,"inc","dec","jmp","bne","red",
    "prn","jsr","rts","hlt"
};    

char *registers[NUM_OF_REGS] = {
    "r1","r2","r3","r4","r5",
    "r6","r7" ,"PSW","PC"
};

int FR = 1;                                                                         /* Rotation flag */
int curr_line_number = 1;
int IC = 1;
int DC = 1;
int err_flag = 0;
label_node *lbl_head = NULL;                                                        /* Label table head */
dc_node *dc_head = NULL;                                                            /* Data segment head */
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
    if (argc < MIN_ARGV){                                                           /* no arguments */ 
        fprintf(stderr, "error: no arguments.\n");
        exit(1);
    }
    file_count++ ;                                                                  /* argumants found. increase file count */
    strcpy(clean_file_name, argv[1]); /* getting file name */


    /* PreAssembler */
    fp = openFile(clean_file_name,0);
    preAssembler(fp,clean_file_name) ;
    fclose(fp);

    /* First & Second Rotation (FR Flag) */
    fp = openFile(clean_file_name,1);
    scan_file(fp);



    printList(1);
    printf("DC is: %d\n",DC);
    return 0;
}

