#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"

/*
*   This function creates a row_node
*/
row_node *create_row(int my_row){
    row_node *new_row = malloc(sizeof(row_node));                           /* Allocates memory for the new node */
    check_allocation(new_row);                                              /* Checks to see if the memory was allocated correctly */
    new_row -> value = my_row; 
    new_row -> next_row = NULL; 
    return new_row;
}

/*
*   This function creates a label_node
* @PARAMS:
*char *line - The entire line of the label
*char *label_name - The name of the label 
*int line_initialized - The line it was intialized in
*enum Label_Type label_type- The type of the label
*union Data data - The data to be stored. If it is an array of numbers, must receive the entire array!!
*/
label_node *create_label(char *line,char *label_name,int line_initialized,enum Label_Type label_type,union Data data){
    label_node *new_label = malloc(sizeof(label_node));                                /* Initialize room for a new word node */
    check_allocation(new_label);
    strcpy(new_label -> line,line);                                                   /* Copies the line to the line varriable in the new label node */

    new_label -> label_name = malloc(strlen(label_name) + 1);                         /* Initialize room for a new label_name string */
    check_allocation(new_label -> label_name);
    strcpy(new_label -> label_name, label_name);                                      /* Store a copy of the label_name */

    new_label -> line_initialized = line_initialized;
    new_label -> label_type = label_type;

    if (label_type == STRING_LABEL) { /* If it is a string label, copies into data.string*/
        strcpy(new_label -> data.string, data.string);
    } else if (label_type == DATA_LABEL || label_type == DEFINE_LABEL) { /* Else, it is a define or an array, therefor copies into data.array */
        memcpy(new_label -> data.array, data.array, sizeof(data.array));
    }

    new_label -> next_label = NULL;
    new_label -> row_list_head = NULL;

    return new_label;
}

