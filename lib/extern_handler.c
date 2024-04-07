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
    label_node *new_node;
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
    
    /* If a duplicate exists, and label_type == ENTRY , change entry_count to 1*/
    if (temp != NULL && label_type == ENTRY_LABEL){
        temp -> entry_count = 1;
    }
    
    label_name = malloc(strlen(p_copy));
    check_allocation(label_name);
    strcpy(label_name,p_copy);

    /* Check there is no remainder past the label */
    pointer = strtok(NULL," \t\n");
    if (pointer != NULL){
        error(ERR_EXTRANEOUS_TEXT);
        return;
    }
    
    /* Add the label to the list */
    add_label(curr_line_number,0,label_name,0,label_type);
    new_node = lbl_head;    
    
    /* If it is an entry label, edit the entry_count to 1 */
    if (label_type == ENTRY_LABEL){
        /* If our temp is null, there is nothing to update. */
        if (temp == NULL){
            return;
        }
        else {
            /* Go through the list until we find the node we just added!*/
            while (new_node != NULL && (new_node -> label_name != label_name) && (new_node -> label_type != label_type) ){
                new_node = new_node-> next_label;
            }
            new_node -> line_init = temp -> line_init;                      /* Copy the address initiated from the 'duplicate' */
            new_node -> entry_count = 1;                                    /* edit the entry_count to 1 */
        /*  add_entry(new_node); */
        }
    }
    /*else{
        while (new_node != NULL && (new_node -> label_name != label_name) && (new_node -> label_type) != label_type ){
            new_node = new_node-> next_label;
        }
        add_extern(new_node);
    }*/
}


