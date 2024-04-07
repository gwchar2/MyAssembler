#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"

void scan_file(FILE *file){
    char line[MAX_LINE_LEN];
    char *inputCopy = NULL;
    char *pointer = NULL;
    char *full_line = NULL;
    Label_Type label_type;
    int flag;
    while (1) {
        /* Grab the next line from the file */
        if (fgets(line, MAX_LINE_LEN, file) == NULL) {
            if (feof(file)) {
                free(inputCopy);
                break; 
            }
        }
        inputCopy = (char *)malloc(strlen(line) + 1); 
        check_allocation(inputCopy);
        strcpy(inputCopy, line); 

        pointer = strtok(inputCopy, " \t"); /* Get the first word from the line */
        /*test*/
        printf("Line: %s",inputCopy);
        /* להבנתי, צריך אחרי כל קריאה לאחת מן הפונקציות, לבצע בדיקת של ספירת השגיאות,*/
        switch (check_first_word(pointer)){
            case 0: /* Blank or note line - ignore */
                break; 
            case 1: /* .define .extern or .entry line */
               /* check_symbol(pointer);*/
                curr_line_number++;
                break;
            case 2: /* potential label */
               /* check_label(pointer); */
                curr_line_number++;
                break;
            case 3: /* Everything else */
                /*check_command(line); return 0 if no error found. return 1 if error found. */ 
               /* מיכל פה זה החלק שלך, שימי לב שיש את כל הפוינטרים שביקשת כגלובלים ולכן לא צריך לשלוח אותם לפונקציה*/
                curr_line_number++;
                break;
            default:
                break;

        if (strlen(line) == MAX_LINE_LEN - 1 && line[MAX_LINE_LEN - 2] != '\n') {           /* Checking to see if the array is full without \n */
            errorCode = ERR_SIZE_LEAK;
            error_manager(errorCode);
        
        /* Check to see if the file is shorter than 80 characters if it isnt, go next line, else, handle. */
        if (strlen(line) == MAX_LINE_LEN - 1 && line[MAX_LINE_LEN - 2] != '\n') {                                       /* Checking to see if the array is full without \n */
            error(ERR_SIZE_LEAK);

        }

        
        /* First rotation */
        else{
            inputCopy = malloc(strlen(line) + 1); 
            check_allocation(inputCopy);
            strcpy(inputCopy, line); 
            /* Grab the first word in the line */
            pointer = strtok(inputCopy, " \t\n\r\f\v");                                                                 
            switch (check_first_word(pointer)){
                /* Cases to ignore ( ; \n entry (at the moment))*/
                case 0:                                                                                                
                    break;   
                /* define or externals */                                                                                           
                case 1:                                                
                    label_type = getLabelType(pointer); 
                    if (label_type == EXTERN_LABEL){
                        pointer = strtok(NULL, " \t\n\r\f\v");
                        extern_handler(pointer,label_type); 
                    }
                    else {
                        pointer = strtok(NULL, " \t\n\r\f\v");
                        if (pointer == NULL){
                            error(ERR_MISSING_ARGUMENT);
                            curr_line_number++; 
                            break; 
                        }
                        define_handler(pointer,label_type);
                    }            
                    curr_line_number++;
                    break;

                /* .data .string or cmd labels */    
                case 2:
                    flag = dstring_handler(pointer);                                                                    
                    if (flag == 0){
                        curr_line_number++;
                        break;
                    }
                    else if (flag == 1){
                        curr_line_number++;
                    }
                    else if (flag == 2){
                        int len;
                        inputCopy = malloc(strlen(line) + 1); 
                        check_allocation(inputCopy);
                        strcpy(inputCopy, line);
                        len = strlen(pointer)+1;
                        full_line = inputCopy + len;
                        /*printf("Line = %s\n",full_line);*/
                        printf("%s %s r%d ~~SEND TO MIHAL full_line~~ \n",__FILE__,__FUNCTION__,__LINE__);
                        printf("%s %s r%d It adds the label in dstring_handler/line_101, if cmd not correct, need to delete! \n",__FILE__,__FUNCTION__,__LINE__);
                        curr_line_number++;
                    }
                    break;

                /* Everything else */
                case 3: 
                    printf("MIHAL PUTS HERE %s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
                    curr_line_number++;
                    break;
                default:
                    error(ERR_UNDEFINED_COMMAND);
                    break;
                
            }
        }           
    }
}


