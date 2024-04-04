#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"


void extern_handler(char *pointer,Label_Type label_type){
    char *label_name = NULL;
    char *p_copy = pointer;
    if (!check_label(p_copy,label_type)){
        return;    
    }
    label_name = malloc(strlen(p_copy));
    check_allocation(label_name);
    strcpy(label_name,p_copy);

    pointer = strtok(NULL," \t\n");
    if (pointer != NULL){
        errorCode = ERR_EXTRANEOUS_TEXT;
        error_manager(errorCode);
        return;
    }
    add_label(curr_line_number,0,label_name,0,label_type);
}