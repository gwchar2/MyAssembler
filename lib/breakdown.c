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
            if (feof(file) && FR == 1) {
                FR = 2;
                fseek(file, 0, SEEK_SET); 
                curr_line_number = 1; 
            }
            else if (feof(file) && FR == 2){
                break; 
            }
        }

        /* Check to see if the file is shorter than 80 characters if it isnt, go next line, else, handle. */
        if (strlen(line) == MAX_LINE_LEN - 1 && line[MAX_LINE_LEN - 2] != '\n') {                                       /* Checking to see if the array is full without \n */
            error(ERR_SIZE_LEAK);
        }

        /* First rotation */
        else if (FR == 1){
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
                        define_handler(pointer,label_type);
                    }            
                    curr_line_number++;
                    break;

                /* .data .string or cmd labels */    
                case 2: 
                    flag = dstring_handler(pointer);                                                                    
                    if ( flag == 0)
                        break;
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
                            /*curr_line_number++;*/
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
            free(inputCopy); 
        }
        else if (FR == 2){
            /*printf("%s",line);*/
        }
    }
}


