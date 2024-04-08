#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"


/*
*   This function creates a row_node
*/
row_node *create_row(int address){
    row_node *new_row = malloc(sizeof(row_node));                           /* Allocates memory for the new node */
    check_allocation(new_row);                                              /* Checks to see if the memory was allocated correctly */
    new_row -> address = address; 
    new_row -> next_row = NULL; 
    return new_row;
}

/*
*   This function adds a row to the specific label node 
*/
void *add_row(label_node *cur_label, int address) {
    row_node *new_row = create_row(address); 
    row_node *cur_row = cur_label -> row_list;

    if (cur_row -> address == address ) 
            return (void*)new_row;
    while (cur_row -> next_row != NULL ){
        if (cur_row -> next_row -> address == address ) 
            return (void*)new_row;
        else
            cur_row = cur_row -> next_row;
    }
    cur_row -> next_row = new_row;

    return (void*)new_row;
}