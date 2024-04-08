#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "../include/assembler.h"

/* 
*   This function receives a word and checks what type of label it can potentialy be.
*/
int check_first_word (char *word){
    Label_Type type;
    
    /* We completely ignore notes / empty lines */
    if (word == NULL || *word == '\n' || *word == ';' || word[0] == ';'){                                                     /* Blank or note line - ignore */
        return 0; 
    }
    /* If the first word starts with a . it is either .define .extern .entry or WRONG */
    else if (word[0] == '.'){
        
        type = getLabelType(word);
        if (type == ENTRY_LABEL){  
            curr_line_number++;                                                                        /* ignore entry labels in the first lap */
            return 0;
        }
        if (type == DEF_LABEL || type == EXTERN_LABEL)
            return 1;                                                                                       /* .define or .extern line */
        else{
            error(ERR_UNDEFINED_COMMAND);
            curr_line_number++;
            return 0;
        }
    }
    /* If it ends with : it must be a label. */
    else if (word[strlen(word)-1] == ':'){    
        return 2;                                                                                           
    }
    /* Everything else goes to 3. */
    else 
        return 3;                                                                                           /* Everything else */
}
/*
* This function receives a word and returns the type of label it represents
*/
Label_Type getLabelType(char *pointer){
    char *p_copy = NULL;
    p_copy = malloc(strlen(pointer));
    check_allocation(p_copy);
    strcpy(p_copy,pointer);
    
    if (p_copy == NULL){
        return INVALID;
    }else if (strcmp(p_copy, ".data") == 0) {
        return DATA_LABEL;
    }else if (strcmp(p_copy, ".string") == 0) {
        return STRING_LABEL;
    }else if (strcmp(p_copy, ".extern") == 0) {
        return EXTERN_LABEL;
    }else if (strcmp(p_copy, ".entry") == 0) {
        return ENTRY_LABEL;
    }else if (strcmp(p_copy, ".define") == 0) {
        return DEF_LABEL;
    }else if (checkWordInArray(commands,p_copy)){
        return CMD_LABEL;
    }
    return INVALID; 
    
}


/*
*   Checks if a label is according to the demands
*   Returns 1 if it is, 0 if it isnt.
*/
int check_label(char *p_copy, Label_Type label_type){
    char *pointer;
    label_node *temp;
    if (label_type == DEF_LABEL || label_type == EXTERN_LABEL || label_type == ENTRY_LABEL){
        
        /* Checks to see if 1 <= label <= 31, if the label is a reg / cmd name, if its all alphabetical letters*/
        if (strlen(p_copy) < 1 || strlen(p_copy) > 31 || checkWordInArray(registers,p_copy) == 1 || checkWordInArray(commands,p_copy) == 1 || check_alpha(p_copy) == 0){
            error(ERR_INVALID_LABEL);
            return 0;
        }
        temp = label_exists(p_copy);
        /* If there is a duplicate */
        if (temp != NULL){   
            /* If its we are checking an entry, and its a duplicate with entry/extern/entry_val = 1 */
            if (label_type == ENTRY_LABEL && (temp -> label_type == ENTRY_LABEL || temp -> label_type == EXTERN_LABEL || temp -> entry_count == 1)){
                error(ERR_DUPLICATE_LABEL);                                                                                                 
                return 0;
                }
            else {
                error(ERR_DUPLICATE_LABEL);                                                                                                 
                return 0;
            }
        }
        /* If the string is equal to .define .data or .string - illegal */
        if (strcmp(p_copy,".define") == 0 || strcmp(p_copy,".data") == 0 || strcmp(p_copy,".string") == 0 || (strcmp(p_copy,".entry") == 0 || strcmp(p_copy,".extern") == 0)){
            error(ERR_INVALID_LABEL);
            return 0;
        }
    }
    /* If there label is for a data/cmd/string */
    else if (label_type == CMD_LABEL || label_type == DATA_LABEL || label_type == STRING_LABEL){
        if (strlen(p_copy) < 2 || strlen(p_copy) > 32){
            error(ERR_INVALID_LABEL);
            return 0;
        }
        pointer = malloc(strlen(p_copy)-1);
        check_allocation(pointer);
        strcpy(pointer,p_copy);
        /* Checks to see if it is a legal word */
        if (!check_alpha(pointer) || checkWordInArray(registers,pointer) == 1 || checkWordInArray(commands,pointer) == 1){                        /* Check to see if the label is all alphabetical letters or reg / cmd name*/
            error(ERR_INVALID_LABEL);   
            free(pointer);                     
            return 0;
        }
        /* Checks to see that no pointer exists */
        if (label_exists(pointer) != NULL){                                                                                                        /* checks to see if the label exists. Entries do not exist yet */
            error(ERR_DUPLICATE_LABEL);
            free(pointer);
            return 0;
        }
    }
    return 1;
}

/* 
*   Checks if a string is all alphabetical letters
*   Returns 1 if it is, returns 0 if it is not
*/
int check_alpha(char *pointer){
    int len = strlen(pointer);
    int i;
    for (i = 0; i < len; i++){
        if (!isalpha(pointer[i]))
            return 0;
    }
    return 1;
}

