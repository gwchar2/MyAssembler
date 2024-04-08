#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"


void extern_handler(char *pointer,Label_Type label_type){
    char *label_name = NULL;
    char *p_copy = pointer;
    label_node *temp = NULL;
    if (pointer == NULL){
        error(ERR_MISSING_ARGUMENT);
        return;
    }
    
    /* Check if the label is proper. (Also checks for duplicates)*/
    if (!check_label(p_copy,label_type)){
        return;    
    }
    /* Grab a duplicate if exists */
    temp = label_exists(p_copy);
    
    
    label_name = malloc(strlen(p_copy));
    check_allocation(label_name);
    strcpy(label_name,p_copy);

    /* Check there is no remainder past the label */
    pointer = strtok(NULL," \t\n");
    if (pointer != NULL){
        error(ERR_EXTRANEOUS_TEXT);
        return;
    }
    
    
    /* Change the new entry count to 1 */
    /* If temp = NULL than there is no 'partner' */
    if (temp != NULL){
        add_label(curr_line_number,0,label_name,1,label_type);
        return;
    }
    else {
        add_label(curr_line_number,0,label_name,0,label_type);
        return;
    }

}
/*
*   This function changes the entry count to '1' for the desired label name and type.
*/
void change_count(char *label_name,Label_Type label_type){
    label_node *temp;
    if (label_type == ENTRY_LABEL)
        temp = entry_head;
    else if (label_type == EXTERN_LABEL)
        temp = extern_head;
    else if (label_type == CMD_LABEL)
        temp = lbl_head;
    else 
        temp = dc_head;
    
    while (temp != NULL){
        if (temp -> label_name == label_name){
            if (label_type == CMD_LABEL != temp -> label_type == CMD_LABEL)
                continue;
            else{
                temp -> entry_count = 1;
                return;
            }
        }
        temp = temp -> next_label;
    }
    return;
}