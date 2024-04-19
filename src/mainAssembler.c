/******************************************************************************************/
/*  *********************             MYASSEMBLER PROGRAM            **********************/
/*  *********************                                            **********************/
/*  *********************             mainAssembler.c                **********************/
/*  *********************   	      @authors:     				 **********************/ 
/*  *********************   	      Tommer Toledo: 209706795 		 **********************/
/*  *********************  	          Michal Rabby: 206600686        **********************/
/*  *********************             @date: 19.4.24                 **********************/
/******************************************************************************************/

#include "../include/assembler.h"
char *commands[NUM_OF_CMDS] = {
    "mov","cmp","add","sub","not","clr",
    "lea" ,"inc","dec","jmp","bne","red",
    "prn","jsr","rts","hlt"
};    

char *registers[NUM_OF_REGS] = {
    "r0","r1","r2","r3","r4","r5","r6","r7","PSW","PC"
};

                                                                 
int curr_line_number = 1;
int IC = 1;
int DC = 1;
int err_flag = 0;
char *curr_file;                                                                            /* This pointer will always point to the current file that is open. */
ErrorCode errorCode = 0;                                                                    /* Global error variable */
label_node *cmd_label_head = NULL;                                                          /* A pointer to all labels that represent a command */
label_node *lbl_head = NULL;                                                                /* Label table head */
label_node *entry_head = NULL;                                                              /* Entry list head */
label_node *extern_head = NULL;                                                             /* Extern list head */
label_node *dc_head = NULL;                                                                 /* Data segment list head */                                                          
macro *head = NULL;                                                                         /* Macro list head */
cmd_node *cmd_head = NULL;                                                                  /* Instruction segment head */
pointer_node *ptr_head = NULL;                                                              /* The head of all dynamically allocated strings */

int main(int argc,char *argv[]) {
/******* PLEASE READ THE README BEFORE STARTING. THANK YOU! **********/
    FILE *fp;
    char *clean_file_name = malloc(FILE_NAME_LEN);                                          /* string to hold the name as recieved in command line. no endings. */
    int file_count;                                                             
    check_allocation(clean_file_name);
    /* Checking if we received files to read from! */
    if (argc < MIN_ARGV){                                                        
        error(ERR_FILE_ARGS);
        free(clean_file_name);
        exit(EXIT_FAILURE);
    }else{
        for (file_count = 1; file_count < argc; file_count++ ) {                                                            
            strcpy(clean_file_name, argv[file_count]);                                      /* clean_file_name is the name of the file without any ending */

            /* Start the process by trying to read the .as file */
            fp = openFile(clean_file_name,0);
            if (fp == NULL){
                continue;
            }
            else{ 
                /* PreAssembler */
                preAssembler(fp,clean_file_name);

                /* Handling and translating the code */
                scan_file(clean_file_name);
                
                /* Finish the final translation to binary, and make the files */
                makefiles(clean_file_name);    
                }

            freeLists();
        } 
    } 

    free(fp);
    free(clean_file_name);
    return 0;
}

