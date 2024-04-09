#include "../include/assembler.h"


char *commands[NUM_OF_CMDS] = {
    "mov","cmp","add","sub","not","clr",
    "lea" ,"inc","dec","jmp","bne","red",
    "prn","jsr","rts","hlt"
};    

char *registers[NUM_OF_REGS] = {
    "r0", "r1","r2","r3","r4","r5",
    "r6","r7" ,"PSW","PC"
};

                                                                 
int curr_line_number = 1;
int IC = 1;
int DC = 1;
int err_flag = 0;
ErrorCode errorCode = 0;                                                            /* Global error variable */
char *rest_of_line = NULL;                                                          /* this pointer will always pont to the rest of the input line that wans't proccessed yet. */
char *curr_file;                                                                    /* This pointer will always point to the current file that is open. */
label_node *cmd_label_head = NULL;                                                  /* A pointer to all labels that represent a command */
label_node *lbl_head = NULL;                                                        /* Label table head */
label_node *entry_head = NULL;                                                      /* Entry list head */
label_node *extern_head = NULL;                                                     /* Extern list head */
label_node *dc_head = NULL;                                                         /* Data segment list head */
cmd_node *cmd_head = NULL;                                                          /* Instruction segment head */
cmd_node *new_cmd = NULL;                                                           
macro *head = NULL; 

int main(int argc,char *argv[]) {
    
    FILE *fp;
    char *clean_file_name = (char*)malloc(FILE_NAME_LEN);                          /* string to hold the name as recieved in command line. no endings. */
    int file_count = 0;                                                             
    check_allocation(clean_file_name);
    rest_of_line = malloc(MAX_LINE_LEN);
    check_allocation(rest_of_line);
    
    /* Checking if we received files to read from! */
    if (argc < MIN_ARGV){                                                        
        error(ERR_FILE_ARGS);
        exit(EXIT_FAILURE);
    }
    for (file_count = 1; file_count < argc; file_count++ ) {                                                              
        strcpy(clean_file_name, argv[file_count]);                                           /* clean_file_name is the name of the file without any ending */
        
        /* Start the process by trying to read the .as file */
        fp = openFile(clean_file_name,0);
        if (fp == NULL)
            continue;
        else{ 
            /* PreAssembler */
            preAssembler(fp,clean_file_name) ;
            fclose(fp);

            /* Before we begin, we must note the following:
            Note lines ( ';' ) and empty lines in the file, will not be counted toward IC, DC or as lines in the file!!! */

            /* Handling and translating the code */
            scan_file(clean_file_name);
            fclose(fp);

            /* Fix the entry labels to hold the correct addresses */
            fixEntrys();

             /* Fix the addresses in the Data segment & Labels used in Instruction segment, and lastly, make the files. */
            mergeSegments();

                /*if (!fixCMDs()){ 
                    continue;
                    If there is a label that is used, but it has no label node --> error(ERR_LBL_USE)!!!;
                 }; 
                 In this stage: we do the following:
                 if label used is external -> we add a row to the specific external in extern_list
                 switch label values with updated values from DC + label table. (i think only data segment and command labels matter?)
                 */
/* משימות להמשך 
לעדכן הוספת שורה ללייבלים של אקסטרנל
לעדכן סימני שאלה ברשימת פקודות
לעדכן הדפסת פקודות לקובץ אובגקט שורות 138-165 ב
file_handler
freeLists() */
            /* If we have errors, dont create files, continue to translate next file given by user! */
            if (err_flag != 0){
                error(ERR_ERR_FLAG);
                continue;
            }
            else {
                makefiles(clean_file_name);
            }
        }
    }
        /* The following are just semi-lists (pointers to lists in lists) */
        /*cmd_label_head = NULL;                                           
        entry_head = NULL;                                                     
        extern_head = NULL;                                                    
        dc_head = NULL; 
        curr_line_number = 1;
        IC = 1;
        DC = 1;*
        err_flag = 0;
        /* We will free everything here using the official heads of lists    */
        /*free_lists();   */

/***********************************************************/
/* DELETE THIS SECTION !!!! */
/**********************************************************/
    printf("IC: %d\t DC: %d\n",IC,DC);
    if (lbl_head != NULL)
        printList(1);
    printf("********************************************\n");
    if (extern_head != NULL)
        printf("\n\nExtern head is: [%p] [%s]\n",(void*)extern_head,extern_head->label_name);
    if (dc_head != NULL){
        printf("DC head is: [%p] [%s] printing list: \n",(void*)dc_head,dc_head->label_name);
        printList(2);
    }
    if (cmd_head != NULL){
        printf("CMD head is: [%p] printing list: \n",(void*)cmd_head);
        printList(3);
    }
    
    return 0;
}

