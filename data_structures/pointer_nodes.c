#include "../include/assembler.h"


/*
*   This function creates a pointer node.
*   @ptr - A pointer to a dynamicaly allocated string.
*/
pointer_node *create_pointer(char *ptr){
    pointer_node *new_ptr = malloc(sizeof(pointer_node));                                    /* Initialize room for a new pointer node */
    check_allocation(new_ptr);

    new_ptr -> pointer = ptr;                                                          
    new_ptr -> next_pointer = NULL;
    return new_ptr;
}

/*
*   This function adds a pointer node to the pointer list.
*   @ptr - A pointer to a dynamicaly allocated string.
*/
void *add_ptr(char *pointer){
    pointer_node *new_ptr = create_pointer(pointer);
    pointer_node *current = ptr_head;
    if (ptr_head == NULL) {
        ptr_head = new_ptr;  
    } else {
        while (current -> next_pointer != NULL) {
            current = current -> next_pointer;
        }
        current -> next_pointer = new_ptr;
    }
    return (void*)new_ptr;
}
