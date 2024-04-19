#include "../include/assembler.h"

/*
*   This function deals with the user .as file.
*   It parses through every line and breaks down the commands according to the type & bracket amount.
*   After we figure out how many brackets (if at all) & variables there are in the received input, we use a switch 
*   to parse through the different variables and options.
*/
void check_cmd(char *full_line){
    char *cmd,*pointer,*line_copy,*p_copy;
    int instruction_type,i,squares;
    Instruction instruction;
    Reg_Type regFlags[5] = {IMPLICIT};
    char *regArray[MAX_REGS] = {NULL};
    cmd_node *cmd_node = NULL;
    reg_node *temp_node = NULL;

    line_copy = malloc(strlen(full_line)+1);
    check_allocation(line_copy);
    add_ptr(line_copy);
    strcpy(line_copy,full_line);
    pointer = line_copy;

    /* Grab the first word in the line */
    p_copy = strtok(line_copy," \f\r\t\v\n");
    if (p_copy == NULL){
        error(ERR_MISSING_ARGUMENT);
        return;
    }

    /* Check if the first word is a viable command */
    if (!checkCmds(p_copy)){
        error(ERR_UNDEFINED_COMMAND);
        return;
    }

    /* Store the first word in the line as a cmd char */
    cmd = malloc(strlen(p_copy)+1);
    check_allocation(cmd);
    add_ptr(cmd);
    strcpy(cmd,p_copy);

    /* Get the enum of the instruction */
    instruction = get_cmd(cmd);
    if (instruction <= 3 || instruction == 6)
        instruction_type = 2;
    else   
        instruction_type = 1;

    /* Count the commas in the line according to the command */
    if (countCommas(instruction_type,full_line))
        return;
    cmd_node = add_cmd(instruction);
    cmd_node -> instruction_type = instruction_type;

    /* Go to the remainder of the line */
    pointer = strtok(NULL,"\f\r\t\v\n");

    /* Only RTS or HLT are allowed to have 0 arguments */
    if (pointer == NULL && ((cmd_node -> instruction != HLT && cmd_node -> instruction != RTS))){
        error(ERR_MISSING_ARGUMENT);
        return;
    }

    /* But on the other hand, if it is NOT a null and it is one of the commands, its an error! */
    else if (pointer != NULL && ((cmd_node -> instruction == HLT || cmd_node -> instruction == RTS))){
        error(ERR_EXTRANEOUS_TEXT);
        return;
    }
    /* Unless everything is alright! And than just return. */
    else if (pointer == NULL && ((cmd_node -> instruction == HLT || cmd_node -> instruction == RTS)))
        return;

    /* Checks to see if there is a legal amunt of commas, if there isnt, returns an error.*/
    if (countCommas(cmd_node -> instruction_type , pointer))
        return;
        
    /* If there is, lets remove the comma and replace it with a ' '*/
    else {
        for (i = 0; i< strlen(pointer); i++){
            if (pointer[i] == ',') {
                pointer[i] = ' ';
                break;
            }
        }
    }
    /* Squares returns the amount of addressed labels that we possibly have! (Counts pairs of []), and returns -1 if there is a syntax error! */
    squares = countSquares(cmd_node, pointer);

    if (squares == -1){
        return;
    }
    /* If there is more than 1 pair of brackets, lets swap them to ' '.
       After that, we 'grab' the registers according to the instruction type, and amount of brackets, and place them in regArray */
    else {
        for (i = 0; i<strlen(pointer); i++){
            if (pointer[i] == '[' || pointer[i] == ']')
                pointer[i] = ' ';
        }
        /* Lets fill up the regs array according to the instruction type & bracket amount */
        for (i = 0; i < MAX_REGS && pointer != NULL; i++){
            /* If the instruction type is 1, and we have 0 brackets, the input is only 1 register long & and it can not be more! */
            if (cmd_node -> instruction_type == 1 && squares == 0){
                if (i == 0){
                    pointer = strtok(pointer," \f\r\t\v\n");
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                    pointer = strtok(NULL,"\n");
                }
                if (pointer != NULL){
                    error(ERR_EXTRANEOUS_TEXT);
                    return;
                }
                if (i > 0)
                    regArray[i] = NULL;
            }
            /*If the instruction type is 1, and we have 1 set of brackets, the input is at minimum & maximum 2 tokens long*/
            else if (cmd_node -> instruction_type == 1 && squares == 1){
                if (i == 0){
                    pointer = strtok(pointer," \f\r\t\v\n");
                    regArray[i] = malloc(strlen(pointer)+1);
                    add_ptr(regArray[i]);
                    check_allocation(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                else 
                    pointer = strtok(NULL," \f\r\t\v\n");
                /* If we are null before the minimum required return an error*/
                if (pointer == NULL && i <= 1){
                    error(ERR_MISSING_ARGUMENT);
                    return;
                }
                /* Check for extra text after the maximum allowed */
                else if (pointer != NULL && i > 1){
                    error(ERR_EXTRANEOUS_TEXT);
                    return;
                }
                /* If we are null exactly where its allowed, just keep a NULL pointer. */
                else if (pointer == NULL && (i == 2 || i == 3)) {
                    regArray[i] = NULL;
                }
                else {
                    regArray[i] = malloc(strlen(pointer)+1);
                    add_ptr(regArray[i]);
                    check_allocation(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
            }
            /* If the instruction type is 2, and we have 0 sets of brackets, the input is at minimum & maximum 2 tokens long. */
            else if (cmd_node -> instruction_type == 2 && squares == 0 ){
                if (i == 0){
                    pointer = strtok(pointer," \f\r\t\v\n");
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                else 
                    pointer = strtok(NULL," \f\r\t\v\n");

                if (i <= 1 && pointer != NULL){
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                /* If we are below the minimum requirment of arguments, and the pointer is null - return error. */
                else if (i <= 1 && pointer == NULL){
                    error(ERR_MISSING_ARGUMENT);
                    return;
                }
                /* If we are above the maximum and pointer isnt null, return an error. */
                else if (i > 1 && pointer != NULL){
                    error(ERR_EXTRANEOUS_TEXT);
                    return;
                }
                else if (i == 3 || i == 2){
                    regArray[i] = NULL;
                }
            }
            /* If the instruction type is 2, and we have 1 sets of brackets, we have a minium and maximum of 3 arguments to capture.*/
            else if (cmd_node -> instruction_type == 2 && squares == 1 ){
                if (i == 0){
                    pointer = strtok(pointer," \f\r\t\v\n");
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                else 
                    pointer = strtok(NULL," \f\r\t\v\n");
                if (i <= 2 && pointer != NULL){
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                /* If we are below the minimum requirment of arguments, and the pointer is null - return error. */
                else if (i <= 2 && pointer == NULL){
                    error(ERR_MISSING_ARGUMENT);
                    return;
                }
                /* If we are above the maximum and pointer isnt null, return an error. */
                else if (i > 2 && pointer != NULL){
                    error(ERR_EXTRANEOUS_TEXT);
                    return;
                }
                else if (i == 3){
                    regArray[i] = NULL;
                }
            }
            /* If the instruction type is 2, and we have 2 sets of brackets, we have a minium and maximum of 4 arguments to capture.*/
            else if (cmd_node -> instruction_type == 2 && squares == 2 ){
                if (i == 0){
                    pointer = strtok(pointer," \f\r\t\v\n");
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                else 
                    pointer = strtok(NULL," \f\r\t\v\n");
                if (i <= 3 && pointer != NULL){
                    regArray[i] = malloc(strlen(pointer)+1);
                    check_allocation(regArray[i]);
                    add_ptr(regArray[i]);
                    strcpy(regArray[i],pointer);
                }
                /* If we are below the minimum requirment of arguments, and the pointer is null - return error. */
                else if (i <= 3 && pointer == NULL){
                    error(ERR_MISSING_ARGUMENT);
                    return;
                }
            }

        }

        /* If we are in instruction type 2 with 2 brackets, and we are above the maximum (a 5th argument exists), return an error. */
        if (cmd_node -> instruction_type == 2 && squares == 2 ){
            pointer = strtok(NULL," \f\r\t\v\n");
            if (pointer != NULL){
                error(ERR_EXTRANEOUS_TEXT);
                return;
            }
        }
        
        
        switch (cmd_node -> instruction_type){
            case 1:
                cmd_node -> addressStyleRS = 0;
                switch(squares){
                    /* No brackets & only 1 variable. The options are reg,imm,label or define only.*/
                    case 0: 
                    /* If the instruction is PRN, it can be also an immediate. */
                        if (cmd_node -> instruction == PRN && (checkImmDef(regArray[0]) != 9000)){
                            temp_node = add_reg(cmd_node,IMM,checkImmDef(regArray[0]));
                            cmd_node -> addressStyleRT = 0;
                            temp_node -> RT = 1;
                            return;
                        }
                    /* Everything else, including PRN, can be a register.  */
                        else if (checkIfReg(regArray[0]) != -1){
                            temp_node = add_reg(cmd_node,REG,checkIfReg(regArray[0]));
                            cmd_node -> addressStyleRT = 3;
                            temp_node -> RT = 1;
                            return;
                        }
                    /* Everything else, including PRN, can be a label.  */
                        else if (check_alpha(regArray[0]) == 1){
                            temp_node = add_reg(cmd_node,LABEL,0);
                            temp_node -> RT = 1;
                            temp_node -> label_name = malloc(strlen(regArray[0])+1);
                            check_allocation(temp_node ->  label_name);
                            add_ptr(temp_node -> label_name);
                            strcpy(temp_node -> label_name, regArray[0]);
                            cmd_node -> addressStyleRT = 1;
                            return;
                        }
                        error(ERR_UNDEFINED_REGISTER_TYPE);
                        return;
                    /* 1 set of brackets, only 1 variable.Must be label[imm]. Only a select few of commands can have such variable.. */
                    case 1:
                        if (cmd_node -> instruction != JMP && cmd_node -> instruction != BNE && cmd_node -> instruction != JSR ){
                            if ((check_alpha(regArray[0]) != 1) || (checkLblImmDef(regArray[1]) == 9000)){
                                error(ERR_UNDEFINED_REGISTER_TYPE);
                                return;
                            }
                            else if ((check_alpha(regArray[0]) == 1) && (checkLblImmDef(regArray[1]) != 9000)) {

                                /* Add the label */
                                temp_node = add_reg(cmd_node,IMM_LBL,0);
                                temp_node -> RT = 1;
                                temp_node -> label_name = malloc(strlen(regArray[0])+1);
                                check_allocation(temp_node -> label_name);
                                add_ptr(temp_node -> label_name);
                                strcpy(temp_node -> label_name, regArray[0]);
                                cmd_node -> addressStyleRT = 2;

                                /* Add the immediate */
                                add_reg(cmd_node,IMM,checkLblImmDef(regArray[1]));
                                return;
                            }
                        }
                        error(ERR_UNDEFINED_REGISTER_TYPE);
                        return;
                }
            case 2:
                switch(squares){
                    /* no brackets, 2 variabels, so they are NOT label[imm]*/
                    case 0: 
                        if (cmd_node -> instruction != LEA){
                            for (i = 0; i < 2; i++){
                                /* Check if is a register */
                                if (checkIfReg(regArray[i]) != -1){
                                    temp_node = add_reg(cmd_node,REG,checkIfReg(regArray[i]));
                                    if (i == 0){
                                        temp_node -> RS = 1;
                                        cmd_node -> addressStyleRS = 3;
                                    }else if (i == 1){
                                        temp_node -> RT = 1;
                                        cmd_node -> addressStyleRT = 3;
                                    }
                                }
                                /* Check if is an immediate or a define  */
                                else if (checkImmDef(regArray[i]) != 9000){
                                    if (i == 1 && cmd_node -> instruction == CMP){
                                        temp_node = add_reg(cmd_node,IMM,checkImmDef(regArray[i]));
                                        temp_node -> RT = 1;
                                        cmd_node -> addressStyleRT = 0;
                                    }
                                    else if (i == 0){
                                        temp_node = add_reg(cmd_node,IMM,checkImmDef(regArray[i]));
                                        temp_node -> RS = 1;
                                        cmd_node -> addressStyleRS = 0;
                                    }
                                    else {
                                        error(ERR_UNDEFINED_REGISTER_TYPE);
                                        return;
                                    }
                                }
                                /* Check if it can represent a label */
                                else if (check_alpha(regArray[i]) == 1){
                                    temp_node = add_reg(cmd_node,LABEL,0);
                                    temp_node -> label_name = malloc(strlen(regArray[i])+1);
                                    check_allocation(temp_node ->  label_name);
                                    add_ptr(temp_node -> label_name);
                                    strcpy(temp_node -> label_name, regArray[i]);
                                    if (i == 0){
                                        temp_node -> RS = 1;
                                        cmd_node -> addressStyleRS = 1;
                                    }else if (i == 1){
                                        temp_node -> RT = 1;
                                        cmd_node -> addressStyleRT = 1;
                                    }
                                }
                                /* If it is neither of the above, its an error! */
                                else {
                                    error(ERR_UNDEFINED_REGISTER_TYPE);
                                    return;
                                }
                            }
                        }
                        /* First argument (in this case) must be a label, and the second argument is either a label or a register. */
                        else if (cmd_node -> instruction  != LEA){
                            /* If its not acording to the rules, return an undefined register type. */
                            if (check_alpha(regArray[0]) != 1 || (checkIfReg(regArray[1]) == -1 && check_alpha(regArray[1]) != 1)){
                                error(ERR_UNDEFINED_REGISTER_TYPE);
                                return;
                            }
                            /* If it is, add the register nodes. */
                            else {
                                /* Add the label for register 1. */
                                temp_node = add_reg(cmd_node,LABEL,0);
                                temp_node -> label_name = malloc(strlen(regArray[0])+1);
                                check_allocation(temp_node ->  label_name);
                                add_ptr(temp_node -> label_name);
                                strcpy(temp_node -> label_name, regArray[0]);
                                cmd_node -> addressStyleRS = 1;
                                temp_node -> RS = 1;
                                
                                /* Handle register 2. */
                                if (checkIfReg(regArray[1]) != -1){
                                    temp_node = add_reg(cmd_node,REG,checkIfReg(regArray[1]));
                                    cmd_node -> addressStyleRT = 3;
                                    temp_node -> RT = 1;
                                }
                                else{
                                    temp_node = add_reg(cmd_node,LABEL,0);
                                    temp_node -> label_name = malloc(strlen(regArray[1])+1);
                                    check_allocation(temp_node ->  label_name);
                                    add_ptr(temp_node -> label_name);
                                    strcpy(temp_node -> label_name, regArray[1]);
                                    cmd_node -> addressStyleRT = 1;
                                    temp_node -> RT = 1;
                                }
                            }
                        }
                        break;
                    /* 1 bracket, 2 variables, that means a total of 3 arguments. */
                    
                    case 1: 
                        if (cmd_node -> instruction == CMP){
                            if (checkLblImmDef(regArray[2]) != 9000 && (check_alpha(regArray[1]))){
                                regFlags[2] = IMM_FR_LBL;
                                regFlags[1] = IMM_LBL;
                                if (checkIfReg(regArray[0]))
                                    regFlags[0] = REG;
                                else if (checkImmDef(regArray[0]))
                                    regFlags[0] = IMM;
                                else if (check_alpha(regArray[0]))
                                    regFlags[0] = LABEL;
                            }
                            else if ((checkLblImmDef(regArray[1])!= 9000 && check_alpha(regArray[0]))){
                                if (checkIfReg(regArray[2]) != -1)
                                    regFlags[2] = REG;
                                else if (checkImmDef(regArray[2])){
                                    regFlags[2] = IMM;
                                }
                                else if (check_alpha(regArray[2]))
                                    regFlags[2] = LABEL;
                                regFlags[1] = IMM_FR_LBL;
                                regFlags[0] = IMM_LBL;
                            }
                            else {
                                error(ERR_UNDEFINED_REGISTER_TYPE);
                                return;
                            }
                        }
                        else {
                            for (i = 0; i < 3; i++){
                                if (checkLblImmDef(regArray[i]) != 9000){
                                    regFlags[i] = IMM_FR_LBL;
                                    if (i-1 >= 0 && regFlags[i-1] == LABEL){
                                        regFlags[i-1] = IMM_LBL;
                                    }
                                    else if (i == 2 && check_alpha(regArray[i])){
                                        regFlags[i] = LABEL;   
                                    }
                                    else{
                                        error(ERR_UNDEFINED_REGISTER_TYPE);
                                        return;
                                    }
                                }
                                else if (check_alpha(regArray[i])){
                                    regFlags[i] = LABEL;    
                                }
                                else if (checkImmDef(regArray[i]) != 9000){
                                    regFlags[i] = IMM;
                                }
                                if (checkIfReg(regArray[i]) != -1){
                                    regFlags[i] = REG;
                                }
                            }
                            for (i = 0; i < 3; i++){
                                if (regFlags[i] == IMM_LBL && regFlags[i+1] == IMM_FR_LBL)
                                    regFlags[4] = EXTERN; /* Just a flag */
                            }
                            if (regFlags[4] != EXTERN ){
                                error(ERR_ILLEGAL_ADDRESSING);
                                return;
                            }
                        }
                        for (i = 0; i < 3; i++){
                            if (regFlags[i] == IMM_FR_LBL){  
                                temp_node = add_reg(cmd_node,IMM,checkLblImmDef(regArray[i]));
                                if (i == 2){    
                                    cmd_node -> addressStyleRT = 2;
                                }  
                                else if (i == 1){
                                    cmd_node -> addressStyleRS = 2;
                                } 
                            }
                            else if (regFlags[i] == LABEL){
                                temp_node = add_reg(cmd_node,LABEL,0);
                                temp_node -> label_name = malloc(strlen(regArray[i])+1);
                                check_allocation(temp_node ->  label_name);
                                add_ptr(temp_node -> label_name);
                                strcpy(temp_node -> label_name, regArray[i]);
                                if (i == 0){
                                    cmd_node -> addressStyleRS = 1;
                                    temp_node -> RS = 1;
                                }  
                                else if (i == 2){
                                    cmd_node -> addressStyleRT = 1;
                                    temp_node -> RT = 1;
                                }  
                            }
                            else if (regFlags[i] == REG){
                                temp_node = add_reg(cmd_node,REG,checkIfReg(regArray[i]));
                                if (i == 0){
                                    cmd_node -> addressStyleRS = 3;
                                    temp_node -> RS = 1;
                                }  
                                else if (i == 2){
                                    cmd_node -> addressStyleRT = 3;
                                    temp_node -> RT = 1;
                                } 
                            }
                            else if (regFlags[i] == IMM){
                                temp_node = add_reg(cmd_node,IMM,checkImmDef(regArray[i]));
                                if (i == 0){
                                    cmd_node -> addressStyleRS = 0;
                                    temp_node -> RS = 1;
                                }  
                                else if (i == 2){
                                    cmd_node -> addressStyleRT = 0;
                                    temp_node -> RT = 1;
                                } 
                            }
                            else if (regFlags[i] == IMM_LBL){
                                temp_node = add_reg(cmd_node,LABEL,0);
                                temp_node -> label_name = malloc(strlen(regArray[i])+1);
                                check_allocation(temp_node ->  label_name);
                                add_ptr(temp_node -> label_name);
                                strcpy(temp_node -> label_name, regArray[i]);
                                if (i == 0){
                                    cmd_node -> addressStyleRS = 2;
                                    temp_node -> RS = 1;
                                }  
                                else if (i == 1){
                                    cmd_node -> addressStyleRT = 2;
                                    temp_node -> RT = 1;
                                } 
                            }
                        }
                        break;
                    /* 2 brackets, 2 variables, has to be a combo of label[imm] label[imm] */
                    case 2: 
                        /* Check if even one of them is not valid. */
                        if (check_alpha(regArray[0]) != 1 || check_alpha(regArray[2]) != 1 || checkLblImmDef(regArray[1]) == 9000 || (checkLblImmDef(regArray[3]) == 9000)){
                            error(ERR_UNDEFINED_REGISTER_TYPE);
                            return;
                        }
                        else {
                            /* Add the first label */
                            temp_node = add_reg(cmd_node,LABEL,0);
                            temp_node -> label_name = malloc(strlen(regArray[0])+1);
                            check_allocation(temp_node ->  label_name);
                            add_ptr(temp_node -> label_name);
                            strcpy(temp_node -> label_name, regArray[0]);
                            temp_node -> RS = 1;

                            /* Add the first immediate */
                            temp_node = add_reg(cmd_node,IMM,checkLblImmDef(regArray[1]));
                            
                            /* Add the second label */
                            temp_node = add_reg(cmd_node,LABEL,0);
                            temp_node -> label_name = malloc(strlen(regArray[2])+1);
                            check_allocation(temp_node ->  label_name);
                            add_ptr(temp_node -> label_name);
                            strcpy(temp_node -> label_name, regArray[2]);
                            temp_node -> RT = 1;

                            /* Add the second immediate */
                            temp_node = add_reg(cmd_node,IMM,checkLblImmDef(regArray[3]));
                            
                            cmd_node -> addressStyleRS = 2;
                            cmd_node -> addressStyleRT = 2;
                        }
                        break;
                }

        }

    }
}


/*
*   Returns the enum value of the specific string command.
*/
Instruction get_cmd(char *cmd){
    if (strcmp(cmd, "mov") == 0) {
        return MOV;
    } else if (strcmp(cmd, "cmp") == 0) {
        return CMP;
    } else if (strcmp(cmd, "add") == 0) {
        return ADD;
    } else if (strcmp(cmd, "sub") == 0) {
        return SUB;
    } else if (strcmp(cmd, "not") == 0) {
        return NOT;
    } else if (strcmp(cmd, "clr") == 0) {
        return CLR;
    } else if (strcmp(cmd, "lea") == 0) {
        return LEA;
    } else if (strcmp(cmd, "inc") == 0) {
        return INC;
    } else if (strcmp(cmd, "dec") == 0) {
        return DEC;
    } else if (strcmp(cmd, "jmp") == 0) {
        return JMP;
    } else if (strcmp(cmd, "bne") == 0) {
        return BNE;
    } else if (strcmp(cmd, "red") == 0) {
        return RED;
    } else if (strcmp(cmd, "prn") == 0) {
        return PRN;
    } else if (strcmp(cmd, "jsr") == 0) {
        return JSR;
    } else if (strcmp(cmd, "rts") == 0) {
        return RTS;
    } else if (strcmp(cmd, "hlt") == 0) {
        return HLT;
    } else {
        return UNKNOWN;
    }
}

/*  
* This function checks if the number of commas in the command line is valid.
*   @param – instruction_type – number of operands required for this command 
*   @param – full_line – the full line from the source file. 
*   @return – 0 if valid. 1 if invalid
*/
int countCommas(int instruction_type, char *full_line){
    int i,total_comma,size_remainder;
    char *remainder_copy;
    size_remainder = strlen(full_line);
    remainder_copy = malloc(strlen(full_line) +1);
    check_allocation(remainder_copy);
    add_ptr(remainder_copy);
    strcpy(remainder_copy,full_line);
    total_comma = 0;

    /* Checks for extra and consecutive commas */
    for (i = 0; i < size_remainder; i++){            
        if (!WHITESPACE(remainder_copy[i])){
            if (remainder_copy[i] == ','){
                if (instruction_type == 1){
                    error(ERR_ILLEGAL_COMMA);
                    return 1;
                }
                if (i == 0 || i == size_remainder-1){
                    error(ERR_ILLEGAL_COMMA);
                    return 1;
                }
                total_comma++;
                if (total_comma >= 2){
                    error(ERR_MULTIPLE_CONSECUTIVE_COMMAS);
                    return 1;
                }
            }
        }
    }
    
    if (total_comma == 0 && instruction_type == 2){
        error(ERR_MISSING_COMMA);
        return 1;
    }
    return 0;
    
}

/*  
* This function counts the pairs of square braces [ ] in a command line.
*   @param – cmd_node – the current command node
*   @param – full_line – the full line from the source file. 
*   @return – (-1) for error. if valid, returns the number of pairs
*/
int countSquares(cmd_node *cmd_node, char *full_line){
    int i,j;
    int size_remainder = strlen(full_line)+1;
    int opencounter = 0;
    int closecounter = 0;
    int combocounter = 0;
    /* Checks for [ and ] */
    for (i = 0; i < size_remainder; i++){            
        if (i == 0 && (full_line[i] == ']' || full_line[i] == '[')){
            error(ERR_ILLEGAL_BRACKETS);
            return -1;
        }
        if (full_line[i] == ']' && combocounter == 0){
            error(ERR_ILLEGAL_BRACKETS);
            return -1;
        }
        else if (full_line[i] == ']'){
            j = i-1;
            /* Lets check if its something like [   ] , if it is, error!*/
            while (WHITESPACE(full_line[j]) && j > 0 ){
                j--;
            }
            if (full_line[j] == '[' || j == 0){
                error(ERR_ILLEGAL_BRACKETS);
                return -1;
            }
            combocounter--;
            closecounter++;
        }
        else if (full_line[i] == '['){
            j = i+1;
            /* Lets check if its something like [   ] , if it is, error!*/
            while (WHITESPACE(full_line[j]) && j < size_remainder ){
                j++;
            }
            if (full_line[j] == ']' || j == 0){
                error(ERR_ILLEGAL_BRACKETS);
                return -1;
            }
            /* We now need to make sure there is no whitespace inbetween the label and the [ */
            j = i;
            if (WHITESPACE(full_line[j-1])){
                error(ERR_ILLEGAL_BRACKETS);
                return -1;
            }
            combocounter++;
            opencounter++;
        }
        
    }
    
    /* If the opening / closing counters are not equal (combo = 0 in that case) return an error.*/
    if (combocounter != 0 ){
        error(ERR_ILLEGAL_BRACKETS);
        return -1;
    }
    /* If there is more than 1 set of brackets, it is legal only if the instruction type receives 2 variables. */
    else if ((opencounter >= 2 && cmd_node -> instruction_type == 1)){
        error(ERR_UNDEFINED_REGISTER_TYPE);
        return -1;
    }
    /* If there is a set of brackets and the command is a JMP BNE or JSR - illegal! */
    else if (opencounter >= 1 && (cmd_node -> instruction == JMP || cmd_node -> instruction == BNE || cmd_node -> instruction == JSR)){
        error(ERR_UNDEFINED_REGISTER_TYPE);
        return -1;
    }
    /* If there are more than 2 sets, illegal!*/
    else if (opencounter > 2){
        error(ERR_UNDEFINED_REGISTER_TYPE);
        return -1;
    }   

    return opencounter;
}


/*  
* This function checks if a given string is a valid register name to use in command lines.
*   @param – reg –  a string to look for in registers array 
*   @return – reg number found. (-1)  if not found
*/
int countRegs(char *pointer){
    int counter = 0;
    pointer = strtok(pointer," \f\r\t\v\n");
    while (pointer != NULL){
        counter++;
        pointer = strtok(NULL," \f\r\t\v\n");
    }
    return counter;
}

/* 
*   This function checks if a received string represents a valid register.
*   If yes, it returns the number of the register. Else, it returns -1.
*/
int checkIfReg(char *reg){
    int i = 0;
    for (i = 0; i < 8; i++){
        if (strcmp(registers[i],reg) == 0)
            return i;
    }
    return -1;
}

/*  
* This function checks if a received string represents a valid define or immediate.
*   @param – reg – the string to check 
*   @return – if valid - the Immidiate value, else 9000 
*/
int checkImmDef(char *reg){
    label_node *some_label = NULL;

    /* Check if the first char is # */
    if (*reg != '#')
        return 9000;

    reg++;
    /* Need to check if the first char is a number or a character. If its a number, it cant be a define, and vice versa. */
    if (isalpha(*reg)){
        some_label = label_exists(reg);
        if (some_label != NULL && some_label -> label_type == DEF_LABEL){
            return some_label -> definedData;
        }
    }
    else if (strToInt(reg) < MAX_12BITS && strToInt(reg) > MIN_12BITS)
        return strToInt(reg);

    return 9000;
}

/*  
* Since an immediate or a define that comes as an index for a label, doesnt have to have #
* This function checks if its valid or not. 
*   @param – reg – the string to check.
*   @return – if valid - the Immidiate value, else 9000 
*/
int checkLblImmDef(char *reg){
    label_node *some_label = NULL;
   /* Need to check if the first char is a number or a character. If its a number, it cant be a define, and vice versa. */
    if (isalpha(*reg)){
        some_label = label_exists(reg);
        if (some_label != NULL && some_label -> label_type == DEF_LABEL){
            return some_label -> definedData;
        }
    }
    else if (strToInt(reg) < MAX_12BITS && strToInt(reg) > MIN_12BITS)
        return strToInt(reg);

    return 9000;
}
