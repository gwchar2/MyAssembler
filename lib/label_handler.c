#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"

/* 
*   This function receives a word and checks what type of label it can potentialy be.
*/
int check_first_word (char *word){
    Label_Type type;
    if (word == NULL || *word == '\n' || *word == ';' || word[0] == ';'){                                                     /* Blank or note line - ignore */
        return 0; 
    }
    else if (word[0] == '.'){
        type = getLabelType(word);
        if (type == ENTRY_LABEL){                                                                           /* ignore entry labels in the first lap */
            return 0;
        }
        if (type == DEF_LABEL || type == EXTERNAL_LABEL)
            return 1;                                                                                       /* .define or .extern line */
        else{
            errorCode = ERR_UNDEFINED_COMMAND;
            error_manager(errorCode);
            return 0;
        }
    }
    else if (word[strlen(word)-1] == ':'){
        return 2;                                                                                           /* potential label */
    }
    else 
        return 3;                                                                                           /* Everything else */
}
/*
* This function receives a word and returns the type of label it represents
*/
Label_Type getLabelType(char *pointer){
    if (strcmp(pointer, ".data") == 0) {
        return DATA_LABEL;
    } else if (strcmp(pointer, ".string") == 0) {
        return STRING_LABEL;
    }else if (strcmp(pointer, ".extern") == 0) {
        return EXTERNAL_LABEL;
    }    else if (strcmp(pointer, ".entry") == 0) {
        return ENTRY_LABEL;
    }else if (strcmp(pointer, ".define") == 0) {
        return DEF_LABEL;
    }else if (checkWordInArray(commands,pointer)){
        return CMD_LABEL;
    }
    else {
        return INVALID; 
    } 
}


