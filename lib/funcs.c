#include "../include/assembler.h"

/* 
This function checks if a given string is a register name.
*   @param – targetWord –  a string to look for in registers array 
*   @return – 1 if found in registers array. 0 if not
*/
int checkRegs(char *targetWord){
    int i;
    for (i = 0; i < NUM_OF_REGS; i++){
        if (strcmp(registers[i], targetWord) == 0){
            return 1; 
        }
    }   
    return 0;
}

/* 
* This function checks if a given string is a command name.
*   @param – targetWord –  a string to look for in commands array 
*   @return – 1 if found in commands array. 0 if not
*/
int checkCmds(char *targetWord){
    int i;
    for (i = 0; i < NUM_OF_CMDS; i++){
        if (strcmp(commands[i], targetWord) == 0){
            return 1; 
        }
    }   
    return 0;
}

/*
*   Calculates the given string to an int value.
*   If the int calculated is a greater value than what can be displayed with 14 bits in signed magnitude, 
*   Or if the string is not entirely numbers, returns 9000
*/
int strToInt(char *string){
    int len,i = 0;
    int flag = 0;
    if (string[0] == '-'){
        flag = -1;
    }
    else if (string[0] == '+'){
        flag = 1;
    }
    if (flag != 0)
        string = string+1;
    else 
        flag = 1;
    len = strlen(string);
    for (i = 0; i < len; i++){
        if (string[i] > 57 || string[i] < 48)
            return 9000;
    }

    
    if (flag*atoi(string) > 8191 || flag*atoi(string) < -8191)
        return 9000;
    return flag*atoi(string);

}

/* 
* This function frees the memory of all the linked list created in the program.
* Frees the label list and sub data list.
* Frees the command list and sub reg list.
* Frees the macro list and sub row list.
*/

void freeLists(){
    label_node *current = lbl_head;
    cmd_node *curcmd = cmd_head;
    macro *curmac = head;
    pointer_node *curptr = ptr_head;
    label_node *temp = NULL;
    cmd_node *curtemp = NULL;
    macro *mactemp = NULL;
    pointer_node *ptrtemp = NULL;
    while (current != NULL){
        free(current -> label_name);
        freeDataList(current -> data_node);
        freeRowList(current -> row_list);
        temp = current;
        current = current -> next_label;
        free(temp);
    }
    while (curcmd != NULL){
        free(curcmd -> bin_value);
        freeRegs(curcmd -> next_reg);
        curtemp = curcmd;
        curcmd = curcmd -> next_cmd;
        free(curtemp);
    }
    while (curmac != NULL){
        free(curmac -> mac_name);
        freeMacro(curmac -> text);
        mactemp = curmac;
        curmac = curmac -> next;
        free(mactemp);
    }
    while (curptr != NULL){
        free(curptr -> pointer);
        ptrtemp = curptr;
        curptr = curptr -> next_pointer;
        free(ptrtemp);
    }

    resetGlobals();
}
/*
*   Frees the macro list
*/
void freeMacro(mac_text *head){
    mac_text *current = head;
    mac_text *temp = NULL;
    while (current != NULL) {
        free(current -> text);
        temp = current;
        current = current -> next;
        free(temp);
    }
}
/*
*   Frees the sub register List
*/
void freeRegs(reg_node *head){
    reg_node *temp = NULL;
    reg_node *current = head;
    while (current != NULL) {
        free(current -> bin_value);
        temp = current;
        current = current -> next_reg;
        free(temp);
    }
}
/*
*   Frees the sub data List
*/
void freeDataList(data_node *head){
    data_node *temp = NULL;
    data_node *current = head;
    while (current != NULL) {
        temp = current;
        current = current -> next_data;
        free(temp);
    }
}
/*
*   Frees the sub row list
*/
void freeRowList(row_node *head) {
    row_node *current = head;
    row_node *temp = NULL;
    while (current != NULL) {
        temp = current;
        current = current -> next_row;
        free(temp);
    }
}

/*  
*   This function resets all the global variables in the program in order to start processing another file. 
*/
void resetGlobals(){
    curr_line_number = 1;
    IC = 1;
    DC = 1;
    err_flag = 0;                                                                   
    cmd_label_head = NULL;                                                  
    lbl_head = NULL;                                                        
    entry_head = NULL;                                                      
    extern_head = NULL;                                                    
    dc_head = NULL;                                                                                                             
    head = NULL; 
    cmd_head = NULL; 
    curr_file = NULL;
    ptr_head = NULL;
}