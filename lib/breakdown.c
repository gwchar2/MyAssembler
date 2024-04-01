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
    while (1) {
        if (fgets(line, MAX_LINE_LEN, file) == NULL) {
            if (feof(file)) {
                break;
            }
        }
        inputCopy = (char *)malloc(strlen(line) + 1); 
        check_allocation(inputCopy);
        strcpy(inputCopy, line); 

        pointer = strtok(inputCopy, " \t"); /* Get the first word from the line */
        /*test*/
        printf("Line: %s",inputCopy);
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
                /*check_command(); */
               /* מיכל פה זה החלק שלך, שימי לב שיש את כל הפוינטרים שביקשת כגלובלים ולכן לא צריך לשלוח אותם לפונקציה*/
                curr_line_number++;
                break;
            default:
                break;
        }

        free(inputCopy); 
    }
}

int check_first_word (char *word){
    if (word == NULL || *word == '\n' || *word == ';'){
        return 0; /* Blank or note line - ignore */
    }
    else if (word[0] == '.'){
        return 1; /* .define .extern or .entry line */
    }
    else if (word[strlen(word)-1] == ':'){
        return 2; /* potential label */
    }
    else 
        return 3; /* Everything else */
}
