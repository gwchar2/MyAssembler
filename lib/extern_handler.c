#include "../include/assembler.h"


/*
*   This function handles all .external and .entry labels
*   @pointer - The remainding line 
*   @label_type - The type (entry or external) 
*/
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
    
    
    label_name = malloc(strlen(p_copy)+1);
    check_allocation(label_name);
    add_ptr(label_name);
    strcpy(label_name,p_copy);

    /* Check there is no remainder past the label */
    pointer = strtok(NULL," \f\r\t\v\n");
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
