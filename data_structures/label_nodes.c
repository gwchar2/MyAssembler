#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"


/*
*   This function creates a word_node according to the label name, type, initiated address and sets everything else to NULL
*/
label_node *create_label(int line_init, char *label_name,Label_Type label_type){
    label_node *new_label = malloc(sizeof(label_node));                        /* Initialize room for a new label node */
    check_allocation(new_label);
    new_label -> label_name = malloc(strlen(label_name) + 1);                         /* Initialize room for a new label string */
    check_allocation(new_label -> label_name);
    strcpy(new_label -> label_name, label_name);                                      /* Store a copy of the label name */

    new_label -> line_init = line_init;
    new_label -> label_type = label_type;
    new_label -> definedData = -1;
    new_label -> data_count = 0;
    new_label -> data = NULL;
    new_label -> row_list = NULL;
    new_label -> next_label = NULL;
    new_label -> prev_label = NULL;
    return new_label;
}
/*
*   This function adds a label to the list
 */
void add_label (int line_init, char *label_name,Label_Type label_type) {
    label_node *new_label = create_label(line_init, label_name, label_type);                           /* Creates a new node */
    label_node *temp = lbl_head; 
    if (lbl_head == NULL) {                                                   /* If the list is empty, add the new node to the top of the list */
        lbl_head = new_label;
        return;
    }
    else {
        while (temp -> next_label != NULL) {
            temp = temp -> next_label;      
        }  
        temp -> next_label = new_label;
        new_label -> prev_label = temp;   
        return;
    }
        
    return;
}

/*
*   This function checks if a label exists in the data list 
*/
label_node *label_exists(char *curr_label) {
    label_node *temp = lbl_head;
    if (lbl_head==NULL)
        return NULL;                                                        /* Returns if the list is empty */
    while (temp != NULL) {
        if (strcmp(temp -> label_name,curr_label) == 0)                            /* Goes through the list and compares the names */
            return temp;
        temp = temp -> next_label; 
    }
    return NULL;
}




/*
*   This function receives a variable and prints the coresponding list. 
*   Enter 1 for label list, 2 for dc list, and 3 for instruction list.
*/
void printList(int num){
    label_node *temp = NULL;
    cmd_node *cmd_temp = NULL;
    data *data_temp = NULL; 
    row_node *row_temp = NULL; 
    switch (num){
        case 1: /* label list */
            temp = lbl_head;
            while (temp != NULL){
                printf("[%s] is of type [%d]\n",temp -> label_name,temp -> label_type);
                printf("[%s] appears in rows\n",temp -> label_name);
                row_temp = temp -> row_list;
                while (row_temp != NULL){
                    if (row_temp -> next_row == NULL)
                        printf("%d\n",row_temp -> address);
                    else
                        printf("%d,",row_temp -> address);
                    row_temp = row_temp -> next_row;
                }
                temp = temp -> next_label;
            }
            temp = lbl_head;
            while (temp != NULL){
                printf("[%s] Holds the following data \n",temp -> label_name);
                if (temp -> label_type == DEF_LABEL){
                    printf("%d",temp -> definedData);
                }
                else {
                    data_temp = temp -> data;
                    while (data_temp != NULL){
                        if (data_temp -> next_data == NULL)
                            printf("%d\n",data_temp -> data);
                        else
                            printf("%d,",data_temp -> data);
                        data_temp = data_temp -> next_data;
                    }
                    temp = temp -> next_label;
                }
            }
            break;
        case 2: /* dc list */
            temp = dc_head;
            while (temp != NULL){
                printf("[%s] is of type [%d]\n",temp -> label_name,temp -> label_type);
                printf("[%s] appears in rows\n",temp -> label_name);
                row_temp = temp -> row_list;
                while (row_temp != NULL){
                    if (row_temp -> next_row == NULL)
                        printf("%d\n",row_temp -> address);
                    else
                        printf("%d,",row_temp -> address);
                    row_temp = row_temp -> next_row;
                }
                temp = temp -> next_label;
            }
            temp = dc_head;
            while (temp != NULL){
                printf("[%s] Holds the following data\n",temp -> label_name);
                data_temp = temp -> data;
                while (data_temp != NULL){
                    if (data_temp -> next_data == NULL)
                        printf("%d\n",data_temp -> data);
                    else
                        printf("%d,",data_temp -> data);
                    data_temp = data_temp -> next_data;
                }
                temp = temp -> next_label;
            }
            break;
        case 3: /* cmd list */
            cmd_temp = cmd_head;
            while (cmd_temp != NULL && cmd_temp->next_label != dc_head){
                printf("Address [%d] holds [%d] variables\n",cmd_temp -> address,cmd_temp -> total_vars);
                printf("binary_cmd: %s\t",cmd_temp -> binary_cmd);
                printf("var1_binary: %s\t",cmd_temp -> var1_binary);
                printf("var2_binary: %s\t",cmd_temp -> var2_binary);
                printf("var3_binary: %s\t",cmd_temp -> var3_binary);
                printf("var4_binary: %s\n",cmd_temp -> var4_binary);
                cmd_temp = cmd_temp -> next_cmd;
            }
            break;
        default:
            break;
    }
    return;
}

/*
*   This function frees the dynamicaly allocated nodes in the list.
*   Enter 1 for label list, 2 for dc list, and 3 for instruction list.
*/
void free_list(int num) {
    label_node *current_label = lbl_head;
    label_node *temp_label = NULL;
    row_node *current_row = NULL;
    row_node *temp_row = NULL;
    data *current_data = NULL;
    data *temp_data = NULL;
    cmd_node *current_cmd = NULL;
    cmd_node *temp_cmd = NULL;
    switch(num){
        case 1:
            current_label = lbl_head;
            temp_label = NULL;
            current_row = NULL;
            temp_row = NULL;
            current_data = NULL;
            temp_data = NULL;
            while (current_label != NULL) {
                current_row = current_label->row_list;
                current_data = current_label -> data;
                while (current_row != NULL) {
                    temp_row = current_row->next_row;
                    free(current_row);
                    current_row = temp_row;
                }
                while (current_data != NULL) {
                    temp_data = current_data->next_data;
                    free(current_data);
                    current_data = temp_data;
                }
                temp_label = current_label->next_label;
                free(current_label);
                current_label = temp_label;
            }
            break;
        case 2:
            current_label = dc_head;
            temp_label = NULL;
            current_row = NULL;
            temp_row = NULL;
            current_data = NULL;
            temp_data = NULL;
            while (current_label != NULL) {
                current_row = current_label->row_list;
                current_data = current_label->data;
                while (current_row != NULL) {
                    temp_row = current_row->next_row;
                    free(current_row);
                    current_row = temp_row;
                }
                while (current_data != NULL) {
                    temp_data = current_data->next_data;
                    free(current_data);
                    current_data = temp_data;
                }
                temp_label = current_label->next_label;
                free(current_label);
                current_label = temp_label;
            }
            break;
        case 3:     
            current_cmd = cmd_head;
            temp_cmd = NULL;
            while (current_cmd != NULL && current_cmd->next_label == NULL) {
                temp_cmd = current_cmd->next_cmd;
                free(current_cmd);
                current_cmd = temp_cmd;
            }
            break;
        default:
            break;
    }
    return;

}
