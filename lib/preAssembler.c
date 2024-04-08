#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/assembler.h"


macro *head = NULL ; 

void preAssembler(FILE *fp, char* clean_file_name) {
    char *file_name = NULL ;
    char *temp_name = NULL ; 
    char cur_line[MAX_LINE_LEN] ;
    char copy_line[MAX_LINE_LEN] ;
    char *token = NULL ;
    FILE *newP ;
    macro *temp_mac = NULL ;


    /* create a new .am file with the matching file name */
    /* adding the .am ending */
    temp_name = addFileEnding(clean_file_name,1) ;
    file_name = (char*)malloc(strlen(temp_name)+1) ;/* allocate memory for file name with ending */
    check_allocation(file_name);
    strcpy(file_name,temp_name); /* file name updated with the correct .am ending */

    newP = fopen(file_name,"w");
    check_allocation(newP);

    /* going through the source file, spreading macros to the new .am file */
    while((fgets(cur_line, MAX_LINE_LEN, fp)) != NULL) {
        strcpy(copy_line,cur_line) ; /* copying current line */
        token = strtok(copy_line," \n\t"); /* cut first word */
        temp_mac = searchMcrList(token, head);
        if (temp_mac != NULL) {/* word found in macro list */
            copyMcrText(temp_mac,newP); /* writing macro content in the new file */
            continue ; /* get next line */
        }
        if (token != NULL){
            if (strcmp(token,"mcr") == 0){
                token = strtok(NULL," \n\t"); /* cut macro name */
                if (validateMcrName(token, head)) {/* return 1 if name is new and legal. else, returns 0;  */
                    addMcr(token,fp,&head) ; /* add macro to list with it's content. return after endmcr */
                    continue ;
                }
                else {
                    error(ERR_REDEFINITION_MACRO);
                }
                continue;
            }
        }
        /* no macro defenition or use. copy line as is */
        
        fputs(cur_line,newP) ;
    }
    fclose(newP) ;
}

