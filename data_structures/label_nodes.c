#include "../include/assembler.h"


/*
*   This function creates a label_node according to the label name, type, initiated address and sets everything else to NULL
*   @line_init - Holds the line it as initiated in the file.
*   @definedData - Holds the data of a .define
*   @label_name - Holds the name of the label.
*   @entry_count - A flag for entry labels.
*   @label_type - An enum holding the value corresponding to the type of label we are reciving.
*/
label_node *create_label(int line_init,int definedData,char *label_name,int entry_count,Label_Type label_type){
    label_node *new_label = malloc(sizeof(label_node));                        /* Initialize room for a new label node */
    check_allocation(new_label);
    new_label -> label_name = malloc(strlen(label_name) + 1);                         /* Initialize room for a new label string */
    check_allocation(new_label -> label_name);
    strcpy(new_label -> label_name, label_name);                                      /* Store a copy of the label name */

    new_label -> line_init = line_init;
    new_label -> label_type = label_type;
    new_label -> definedData = definedData;
    new_label -> entry_count = entry_count;
    new_label -> data_count = 0;
    new_label -> data_node = NULL;
    new_label -> row_list = NULL;
    new_label -> next_entry = NULL;
    new_label -> next_extern = NULL;
    new_label -> next_dc = NULL;
    new_label -> next_cmd_label = NULL;
    new_label -> next_label = NULL;
    new_label -> prev_label = NULL;
    return new_label;
}

/*
*   This function adds a label_node according to the label name, type, initiated address and sets everything else to NULL
*   @line_init - Holds the line it as initiated in the file.
*   @definedData - Holds the data of a .define
*   @label_name - Holds the name of the label.
*   @entry_count - A flag for entry labels.
*   @label_type - An enum holding the value corresponding to the type of label we are reciving.
*/
void *add_label (int line_init,int definedData,char *label_name,int entry_count,Label_Type label_type) {
    label_node *new_label = create_label(line_init+99, definedData, label_name, entry_count, label_type);                           /* Creates a new node */
    label_node *temp = lbl_head; 
    if (lbl_head == NULL) {                                                   /* If the list is empty, add the new node to the top of the list */
        lbl_head = new_label;
    }
    else {
        while (temp -> next_label != NULL) {
            temp = temp -> next_label;      
        }  
        temp -> next_label = new_label;
        new_label -> prev_label = temp;  
    }
    if (new_label -> label_type == EXTERN_LABEL)
        add_extern(new_label);
    else if (new_label -> label_type == ENTRY_LABEL)
        add_entry(new_label);
    else if (new_label -> label_type == DATA_LABEL || new_label -> label_type == STRING_LABEL)
        add_dc(new_label);
    else if (new_label -> label_type == CMD_LABEL)
        add_cmd_label(new_label);    
    
    return new_label;
}


/*
*   This function checks if a label exists in the data list.
*   @curr_label - The name of a label we are looking for.
*/
label_node *label_exists(char *curr_label) {
    char *copy = NULL;
    label_node *temp = NULL;
    copy = malloc(strlen(curr_label)+1);
    check_allocation(copy);
    add_ptr(copy);
    strcpy(copy,curr_label);
    temp = lbl_head;
    if (lbl_head==NULL)
        return NULL;                                                                          /* Returns if the list is empty */
    while (temp != NULL) {
        if (strcmp(temp -> label_name,copy) == 0)                                             /* Goes through the list and compares the names */
            return temp;
        temp = temp -> next_label; 
    }
    return NULL;
}

/*
*   This function checks if a label exists in the data list & it is not an entry!
*   @name - The name of a label we are looking for.
*/
void *findNotEntry(char *name){
    label_node *label_temp = NULL;
    if(lbl_head == NULL)
        return NULL;
    else {
        label_temp = lbl_head;
        while (label_temp != NULL){
            if (strcmp(label_temp->label_name,name) == 0 && label_temp -> label_type != ENTRY_LABEL)
                return label_temp;
            label_temp = label_temp -> next_label;
        }
    }
    return NULL;
}

/*
*   This function adds an entry node to the sub-entry list.
*   @some_node - The label node from an entry type we are adding.
*/
void *add_entry(label_node *some_node) {
    label_node *temp = entry_head;
    if (entry_head == NULL) {                                                   /* If the list is empty, add the new node to the top of the list */
        entry_head = some_node;
    }
    else {
        while (temp -> next_entry != NULL) {
            temp = temp -> next_entry;      
        }  
        temp -> next_entry = some_node;
    }
    return temp;
}

/*
*   This function adds an external node to the sub-external list.
*   @some_node - The label node from an external type we are adding.
*/
void *add_extern(label_node *some_node) {
    label_node *temp = extern_head; 
    if (extern_head == NULL) {                                                   /* If the list is empty, add the new node to the top of the list */
        extern_head = some_node;
    }
    else {
        while (temp -> next_extern != NULL) {
            temp = temp -> next_extern;      
        }  
        temp -> next_extern = some_node;
    }
    return temp;
}

/*
*   This function adds a data segment node to the sub-data segment list.
*   @some_node - The label node from an entry type we are adding.
*/
void *add_dc(label_node *some_node){
    label_node *temp = dc_head; 
    if (dc_head == NULL) {                                                   /* If the list is empty, add the new node to the top of the list */
        dc_head = some_node;
    }
    else {
        while (temp -> next_dc != NULL) {
            temp = temp -> next_dc;      
        }  
        temp -> next_dc = some_node;
    }
    return (void*)temp;
}

/*
*   This function adds a cmd label node to the sub - cmd list
*   @some_node - A label node from a command type.
*/
void *add_cmd_label(label_node *some_node){
    label_node *temp = cmd_label_head; 
    if (cmd_label_head == NULL) {                                                   /* If the list is empty, add the new node to the top of the list */
        cmd_label_head = some_node;
    }
    else {
        while (temp -> next_cmd_label != NULL) {
            temp = temp -> next_cmd_label;      
        }  
        temp -> next_cmd_label = some_node;
    }
    return temp;
}



/*
*   This function receives a variable and prints the coresponding list. 
*   Enter 1 for label list, 2 for dc list, and 3 for instruction list.
*   This is specificaly for private use.
*/
void printList(int num){
    label_node *temp = NULL;
    cmd_node *some_cmd = NULL;
    data_node *data_temp = NULL; 
    row_node *row_temp = NULL; 
    label_node *dc_temp = NULL;
    reg_node *some_reg = NULL;
    printf("\n");
    switch (num){
        case 1: /* label list */
            printf("********************************************\n");
            printf("\t\tPrinting label tabel\n");
            printf("********************************************\n");
            temp = lbl_head;
            while (temp != NULL){
                switch (temp -> label_type){
                    case EXTERN_LABEL:
                        printf("[%s]-[%d]-[%d]-",temp -> label_name,temp -> label_type,temp -> line_init);
                        row_temp = temp -> row_list;
                        if (row_temp == NULL){
                            printf("NONE\n");
                        }
                        else{
                            while (row_temp != NULL){
                                if (row_temp -> next_row == NULL)
                                    printf("[%d]\n",row_temp -> address);
                            else
                                printf("[%d]-",row_temp -> address);
                            row_temp = row_temp -> next_row;
                            }
                        }
                        break;
                    case DEF_LABEL:
                        printf("[%s]-[%d]-[%d]\n",temp -> label_name,temp -> label_type,temp -> definedData);
                        break;
                    case ENTRY_LABEL:
                        printf("[%s]-[%d]-[%d]\n",temp -> label_name,temp -> label_type,temp -> entry_count);
                        break;
                    case CMD_LABEL:
                        printf("[%s]-[%d]-[%d]\n",temp -> label_name,temp -> label_type,temp -> line_init);
                        break;
                    default:
                        printf("[%s]-[%d]-[%d]-",temp -> label_name,temp -> label_type,temp -> line_init);
                        data_temp = temp -> data_node;
                        while (data_temp != NULL){
                            if (data_temp -> next_data == NULL)
                                printf("[%d]\n",data_temp -> data);
                            else
                                printf("[%d]-",data_temp -> data);
                            data_temp = data_temp -> next_data;
                        }
                        break;
                }
                if (temp -> next_label != NULL)
                    printf("  |\n");
                else    
                    printf("\n");
                temp = temp -> next_label;
            }
            break;
        case 2: /* dc list */
            dc_temp = dc_head;
            printf("********************************************\n");
            printf("\t\tPrinting DC list\n");
            printf("********************************************\n");
            while (dc_temp != NULL){
                printf("[Name: %s]-[Type: %d]-[Line INIT: %d]-",dc_temp -> label_name,dc_temp -> label_type,dc_temp -> line_init);
                data_temp = dc_temp -> data_node;
                while (data_temp != NULL){
                    if (data_temp -> next_data == NULL)
                        printf("[%d]\n",data_temp -> data);
                    else
                        printf("[%d]-",data_temp -> data);
                    data_temp = data_temp -> next_data;
                    }
                if (dc_temp -> next_dc != NULL)
                    printf("  |\n");
                else    
                    printf("\n");
                dc_temp = dc_temp -> next_dc;
            }
            
            break;
        case 3: /* cmd list */
            some_cmd = cmd_head;
            while (some_cmd != NULL){
                some_reg = some_cmd -> next_reg;
                printf("[IC: %d] -- [INSTRUCTION: %d] -- [TOTAL: %d] ",some_cmd -> address, some_cmd -> instruction,some_cmd -> total_len);
                printf("-- [StyleRS: %d] ",some_cmd -> addressStyleRS);
                printf("-- [StyleRT: %d] ",some_cmd -> addressStyleRT);
                printf("-- [CMDBIN: %s] ",some_cmd -> bin_value);
                printf("\n");
                while (some_reg != NULL){
                    printf("[ARE: %d] -- [REG TYPE: %d] -- [REG VAL: %d] ",some_reg -> ARE,some_reg -> reg_type,some_reg -> data);
                    if (some_reg -> label_name != NULL)
                        printf("-- [LBL NAME: %s] ",some_reg -> label_name);
                    if (some_reg -> RS == 1)
                        printf("-- [TYPE: RS] ");
                    if (some_reg -> RT == 1)
                        printf("-- [TYPE: RT] ");
                    
                    printf("-- [BINVAL: %s] ",some_reg -> bin_value);
                    printf("\n");
                    some_reg = some_reg -> next_reg;
                }
                printf("--------------\n");
                some_cmd = some_cmd -> next_cmd;
            }
            break;
        default:
            break;
    }
    return;
}
