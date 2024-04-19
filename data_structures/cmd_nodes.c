#include "../include/assembler.h"

/*
* This function creates a new command node for the instruction segment list.
    @Instruction - an enum from the instruction list.Represents a valid instruction in the program.
*/
cmd_node *create_cmd(Instruction instruction){
    cmd_node *new_cmd = malloc(sizeof(cmd_node));                       
    check_allocation(new_cmd);

    new_cmd -> bin_value = malloc(BIN_WORD_LEN+1);
    check_allocation(new_cmd -> bin_value);
    new_cmd -> instruction = instruction;                                                            
    new_cmd -> address = IC+99;
    new_cmd -> total_len = 0;
    new_cmd -> next_cmd = NULL;
    new_cmd -> next_reg = NULL;
    
    return new_cmd;
}

/*
* This adds a command node to the instruction segment list.
    @Instruction - an enum from the instruction list.
*/
void *add_cmd(Instruction instruction){
    cmd_node *new_cmd = create_cmd(instruction);                         
    cmd_node *temp = cmd_head; 
    if (cmd_head == NULL) {                                                   
        cmd_head = new_cmd;
    }
    else {
        while (temp -> next_cmd != NULL) {
            temp = temp -> next_cmd;      
        }  
        temp -> next_cmd = new_cmd;
    }
    IC++;
    return (void*)new_cmd;
}

/*
* Creates a reg node for the argument sub-list for the commands.
    @reg_type - An enum describing the type of argument we are receiving
    @value - The value of the register
*/
reg_node *create_reg(Reg_Type reg_type, int value){
    reg_node *new_reg = malloc(sizeof(reg_node));                      
    check_allocation(new_reg);
    
    new_reg -> bin_value = malloc(BIN_WORD_LEN+1);  
    check_allocation(new_reg -> bin_value);

    new_reg -> data = value;                                          
    new_reg -> reg_type = reg_type;
    new_reg -> next_reg = NULL;
    new_reg -> IC = IC;
    new_reg -> row = curr_line_number;
    /* ADD ARE SWITCH  according to reg_type 
       If its an immediate - 00
       If its a register - 00
       If its a label - 10
       If its a label with an immediate - 10
       If its an external label - 01 */
    switch (new_reg -> reg_type ){
        case IMM:
            new_reg -> ARE = 0;
            break;
        case IMM_FR_LBL:
            new_reg -> ARE = 0;
            break;
        case REG:
            new_reg -> ARE = 0;
            break;
        case LABEL:
            new_reg -> ARE = 2;
            break;
        case IMM_LBL:
            new_reg -> ARE = 2;
            break;
        case EXTERN:
            new_reg -> ARE = 1;
            break;
        case IMPLICIT: 
            new_reg -> ARE = -1;
            break;
    }

    return new_reg;
}

/*
*   This function adds a reg node to the sub argument list for commands
    @some_cmd - A command node to add the register too.
    @reg_type - A corresponding value holding the type of register.
    @value - Holds the data stored in the register
*/
reg_node *add_reg (cmd_node *some_cmd, Reg_Type reg_type, int value){
    reg_node *new_reg = create_reg(reg_type,value);
    reg_node *current = some_cmd -> next_reg;

    if (some_cmd -> next_reg == NULL) {
        (some_cmd -> total_len)++;
        IC++;
        some_cmd -> next_reg = new_reg;  
        return (void*)new_reg;
    } 
    else {
        while (current -> next_reg != NULL) {
            current = current -> next_reg;
        }
        current -> next_reg = new_reg;
    }
    if (current -> reg_type == REG && new_reg -> reg_type == REG)    
        return (void*)new_reg;
    else{
        (some_cmd -> total_len)++;
        IC++;
        return (void*)new_reg;
    }
}
