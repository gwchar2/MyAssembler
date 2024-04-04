#include "../include/assembler.h"

cmd_node *create_cmd_node(int cmd_num) {
    cmd_node *result = malloc(sizeof(cmd_node)) ;
    cmd_node *temp = cmd_head ;

    result->cmd_num = cmd_num ;
    result->next = NULL ;
    check_allocation(result);
    result->address = IC ;
    result->L = 1 ;
    result->cmd_binary = malloc(BIN_WORD_LEN+1) ;
    result->source1_binary = malloc(BIN_WORD_LEN+1) ;
    result->source2_binary = malloc(BIN_WORD_LEN+1) ;
    result->target1_binary = malloc(BIN_WORD_LEN+1) ;
    result->target2_binary = malloc(BIN_WORD_LEN+1) ;
    check_allocation(result->cmd_binary);
    check_allocation(result->source1_binary);
    check_allocation(result->source2_binary);
    check_allocation(result->target1_binary);
    check_allocation(result->target2_binary);
    /* connect new cmd to the end of the cmd list */
    if (head == NULL ) /* list is empty */
        head->next = result ;
    else {
        while(temp->next != NULL ){
            temp = temp->next ;
        }
        temp->next = result ;
        }
    result->next_label = NULL ;    
    return result;
}


