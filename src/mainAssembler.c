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

                                                                 
int curr_line_number = 1;
int IC = 1;
int DC = 1;
int err_flag = 0;
label_node *lbl_head = NULL;                                                        /* Label table head */
cmd_node *cmd_head = NULL;                                                          /* Instruction segment head */
label_node *entry_head = NULL;                                                      /* Entry list head */
label_node *extern_head = NULL;                                                     /* Extern list head */
label_node *dc_head = NULL;                                                         /* Data segment list head */
ErrorCode errorCode = 0;                                                            /* Global error variable */


int main(int argc,char *argv[]) {
    
    FILE *fp ;
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
    /* Before we begin, we must note the following:
    Note lines ( ';' ) and empty lines in the file, will not be counted toward IC, DC or as lines in the file!!! */

    fp = openFile(clean_file_name,1);
    scan_file(fp);


    
    printList(1);
    if (extern_head != NULL)
        printf("Extern head is: [%p] [%s]\n",(void*)extern_head,extern_head->label_name);
    printf("Next DC is: %d\n",DC);
    if (dc_head != NULL){
        printf("DC head is: [%p] [%s] printing list: \n",(void*)dc_head,dc_head->label_name);
        printList(2);
    }
    
    
    printf("Need to go a second time for labels. in label_handler->check_label->line 122 ->need to edit the case(maybe?)\n ");
    printf("| Add to entry list via add_label() cmd!! |\n");
    /*printf("\nENTRY HEAD NAME: %s\n",entry_head -> label_name);*/
    /*printf("\CMD HEAD NAME: %s\n",cmd_head ____);*/
    return 0;
}

