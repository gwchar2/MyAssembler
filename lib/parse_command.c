#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/assembler.h"


cmd_node *new_cmd ; 

/* this function is called only if first token of command line is NOT blank/.define/.data/.string/.extern/.entry/pottential LABEL */

int check_command() {
    int error_num = 0 ;
    char *token = NULL ;
    if (error_num = valid_command_name(pointer) != 0) 
        return error_num ;
    getNumOfVars() ;
    /* command name is valid */
    /* check for illegal comma after command name */
    if (*(inputCopy+CMD_NAME_LEN) == COMMA)
        error_num = error_manager(5, curr_line_number) ; /* illegal comma */
    if (error_num != 0)
        return error_num ;
    /* no illegal comma after command name. continue. */ 
    token = strtok(NULL, SPACE_COMMA_DEL) ; /* cut #1 parmeter */

    if (new_cmd->total_vars == FIRST_GROUP_VARS) {
        error_num = sourceOpCheck(token) ; /* send to validation */
        token = strtok(NULL, SPACE_COMMA_DEL) ; /* cut #2 parmeter */
        error_num = targetOpCheck(token) ; /* send to validation */
    }

    if (new_cmd->total_vars == SECOND_GROUP_VARS){ 
        error_num = targetOpCheck(token) ; /* send to validation */
        token = strtok(NULL, SPACE_COMMA_DEL) ; /* cut #2 parmeter */
        if (token != NULL )
            error_num = error_manager(2, curr_line_number) ; /* Extraneous text after end of command in line */
    }

    if (new_cmd->total_vars == THIRD_GROUP_VARS) {
        if (token != NULL )
            error_num = error_manager(2, curr_line_number) ; /* Extraneous text after end of command in line */
    }

}

/* this function gets a word and checks if it is one of the legel commands  if yes - it creates a new command node with the mathing cmd_num. if not - error*/
int valid_command_name(char *cmd) {
    int cmd_num,i ;
    if (strlen(cmd) != CMD_NAME_LEN+1) /* check valid len */
        return error_manager(1,curr_line_number); /* Error: undefined command */
    else {
        /* go through command name array and look for matching command */
        for (i=0; i<NUM_OF_CMDS; i++) {
            if (strcmp(cmd, commands[i])==0) { /* if command name found */
                cmd_num = i; /* save matching command number */
                new_cmd = create_cmd_node(cmd_num) ; /* create a new command node with the matching command number */
                new_cmd->cmd_num = cmd_num ; /* set command num in new_cmd */
                break;
            } 
        }    
        if (i==NUM_OF_CMDS) /* command name not found */
            return error_manager(1,curr_line_number); /* Error: undefined command */
        
    }
    
    return 0; /* no error found */
}

/* this function fill the total_vars feild in the command node based on the command number.  */
void getNumOfVars() {
    int cmd_num = new_cmd->cmd_num ; 
    int vars_req ;  
    switch (cmd_num) {
        case 0:
        case 1:
        case 2:
        case 3:
            vars_req = FIRST_GROUP_VARS;
            break ;
        case 4:
        case 5:
            vars_req = SECOND_GROUP_VARS;
            break ;
        case 6:
            vars_req = FIRST_GROUP_VARS;
            break ;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            vars_req = SECOND_GROUP_VARS;
            break ;
        case 15:
        case 16:
            vars_req = THIRD_GROUP_VARS;
            break ;
        }

}

/* called only if command is from 1st group */
int sourceOpCheck(char *token) {
    int error_num = 0 ;
    size_t len = strlen(token) ;
    int reg_num, immNum ;

    /* Addressing method - 0 */
    if (*token == '#') { /* source op is an immidiate number. */
        if (new_cmd->cmd_num == 6 ) { /* lea command - Imm addressing is illegal */
            error_num = error_manager(10,curr_line_number) ;
            return error_num ;
        }
        error_num = immProcessor(token,&immNum) ; /* validation. if valid, immNum will be the imm to tranlate */
        if (error_num == 0){
            strcpy(new_cmd->source1_binary,immBinTranslation(immNum)) ; /* translate num to 12 bits. last 2 bit are 00 */
            strcpy(new_cmd->source2_binary, '\0') ; /* for imm source op, only 1 word required. */
            (new_cmd->L)++ ; /* increase number of bin words by 1 */
        }
        else
            return error_manager(3,curr_line_number) ;

    }

    /* Addressing method - 2 */
    if (*(token+len-1) == ']') /* source op is an array_index. */
        error_num = indexPrecessor(token) ; /* validation and tranlation */

    /* Addressing method - 1 */
    if (label_exists(token) != NULL) { /* source op is a label. */
        error_num = labelPrecessor(token) ; 
    }

    /* Addressing method - 3 */
    if ((reg_num = isReg(token)) != -1 ) { /* source op is a register. */
        if (new_cmd->cmd_num == 6 ) { /* lea command - Reg addressing is illegal */
            error_num = error_manager(10,curr_line_number) ;
            return error_num ;
        }
        (new_cmd->L)++ ; /* increase number of bin words by 1 */
        strcpy(new_cmd->source1_binary, RSBinTranslation(reg_num)) ;
    }

    /* if addressing method wasn't set by now , illegal source op */
    if ((new_cmd->sourceAdd) == -1 ) {
        error_num = error_manager(3, curr_line_number) ;
        return error_num ;
    }

    return 0 ;
}


/* not ready */
int targetOpCheck(char *token) {
    int error_num = 0 ;
    size_t len = strlen(token) ;
    int reg_num ;

    /* Addressing method - 0 */
    if (*token == '#') { /* source op is an immidiate number. */
        if (new_cmd->cmd_num == 6 ) { /* lea command - Imm addressing is illegal */
            error_num = error_manager(10,curr_line_number) ;
            return error_num ;
        }
        error_num = immProcessor(token) ; /* validation and tranlation */
        strcpy(new_cmd->source1_binary,immBinTranslation(num)) ; /* translate num to 12 bits. last 2 bit are 00 */
        strcpy(new_cmd->source2_binary, '\0') ; /* for imm source op, only 1 word required. */
        (new_cmd->L)++ ; /* increase number of bin words by 1 */
    }

    /* Addressing method - 2 */
    if (*(token+len-1) == ']') /* source op is an array_index. */
        error_num = indexPrecessor(token) ; /* validation and tranlation */

    /* Addressing method - 1 */
    if (label_exists(token) != NULL) { /* source op is a label. */
        error_num = labelPrecessor(token) ; 
    }

    /* Addressing method - 3 */
    if ((reg_num = isReg(token)) != -1 ) { /* source op is a register. */
        if (new_cmd->cmd_num == 6 ) { /* lea command - Reg addressing is illegal */
            error_num = error_manager(10,curr_line_number) ;
            return error_num ;
        }
        (new_cmd->L)++ ; /* increase number of bin words by 1 */
        strcpy(new_cmd->source1_binary, RSBinTranslation(reg_num)) ;
    }

    /* if addressing method wasn't set by now , illegal source op */
    if ((new_cmd->sourceAdd) == -1 ) {
        error_num = error_manager(3, curr_line_number) ;
        return error_num ;
    }

    return 0 ;
}

/* this function is called if the source op statrs with a '#'. validates the imm value, sets addressing method, tranlates source to binary */
int immProcessor(char *token, int *num) {
    char *immP = token+1 ; /* sets on the char after '#' */
    label_node *lblP = NULL ;
    if (immP == NULL) /* blank */
        return error_manager(4, curr_line_number) ; /* missing argumant */
    if (num = isNumber(immP) != NULL ) /* imm is a valid number */
        new_cmd->sourceAdd = 0 ;
    else if ((lblP = label_exists(immP)) != NULL ) { /* imm is a label */
        if (lblP->Label_Type != DEF_LABEL)
            return error_manager(3, curr_line_number) ; /* undefined argument */
        /* label found, type is define. */
        *num = lblP->definedData ;
        new_cmd->sourceAdd = 0 ;
    }
    /* imm is legal. the value is saved in num. continue to bin-tranlation */
    // Ensure the num is within the valid range for 12-bit representation
    if (*num <= MIN_12BITS || *num >= MAX_12BITS ) {
       return error_manager(3, curr_line_number) ;
    }
    return 0 ;
}

int isNumber(char *imm) {

}

/* this functions gets a number and translate it to 12 bits bunary representation. the ARE bits will be set to 00 */
char *immBinTranslation(int num) {
    char result[BIN_WORD_LEN];
    int i, bit, index = 0 ;
    int negHandle = 1 << BITS_IN_INT ; /* for neg numbers, add 4096 to get the correct Two's complement bin representation */

    // Handle negative numbers using Two's complement
    if (num < 0) {
        num += negHandle ;
    }

    // Store the binary representation of num in the string
    for (i = BITS_IN_INT - 1; i >= 0; i--) {
        bit = (num >> i) & 1;
        result[index] = bit + '0'; // Convert the bit to ASCII '0' or '1'
        index++ ;
    }
    /* set the ARE feild to 00 */
    result[index] = '0' ;
    result[index+1] = '0' ;
    result[index+2] = '\0'; // Null terminate the string

    return result ;
}

int validateInput(data *my_data, Mat *mats, char *commands){
    char *token = strtok(my_data->input,SPACE_COMMA_DEL) ; /* cut the command part */
    int error_num = 0 ;
    size_t len ;
    if (token == NULL) /* if command line is empty, move to the next command. */ 
        return 0;
    error_num = commandCheck(token, my_data) ; /* send to command name validation */
    if (error_num!=0) 
        return error_num ;
    else { /* command name is valid */
        /* check for illegal comma after command name */
        len = strspn(my_data->input_copy," my_data->cmd_name") ;
        if (*((my_data->input_copy)+len)==COMMA)
            return 6 ; /* illegal comma */
        /* no illegal comma after command name. continue. */
        //////////////////////////////////
        token = strtok(NULL, SPACE_COMMA_DEL) ; /* cut #1 parmeter */
        error_num = parm1Check(token, my_data, mats) ; /* send to validation */
        if (error_num!=0)
            return error_num ;
        else {
            if (my_data->cmd_num==0)  /* if command is "read_mat", and parm1 is valid, call special validation. */
                return (readCheck(my_data)) ;
            token = strtok(NULL, SPACE_COMMA_DEL) ; /* cut #2 parmeter */
            error_num = parm2Check(token, my_data, mats) ; /* send to validation */
            if (error_num!=0)
                return error_num ;
            else {
                token = strtok(NULL, SPACE_COMMA_DEL) ; /* cut #3 parmeter */
                error_num = parm3Check(token, my_data, mats) ;
                if (error_num!=0)
                    return error_num ;
            }       
        }
    }
    /* command name and parms are valid -> error_num == 0 */
    error_num = commaCheck(my_data) ; /* send to comma validation */
    return error_num ;
}

int isReg(char *token) {
    int i ;
    for (i=0; i<NUM_OF_REGS-2; i++) { /* last 2 registers are not allowed to be used on commands */
        if (strcpm(token, register[i]) == 0 )  /* register found */
            return i ;
    }
    return -1 ;
}

char *RSBinTranslation(int reg_num) {
    char result[BIN_WOD_LEN] ;
    int num = reg_num << RS_FIELD ; /* shifting left to get the RS number in the 5-7 bits */
    strcpy(result, immBinTranslation(num)) ; /* use imm translation func */  
    return result ;
}




/* This function gets the 3rd cut of the input and checks validation of #2 parmeter according to the command name 
    If valid - saves parm2 in my_data struct and returns 0.
    If invalid - returns error number 
    @param mat_num - matrix number in mats array 
    @param type - type of command name in terms of requirments for the 1st parameter 
    @param scalar - for mul_scalar command - the scalar to multiply by. 
    @param cmd_type - enum to classify command types 
    @param end_p - pointer to the rest of the string after seperating the numbers part.  
    @return int - the error number founded. */
int parm2Check(char *parm2, data *my_data, Mat *mats) {
    int mat_num ,type ;
    double scalar ;
    enum cmd_type {print_case, mul_scalar_case, def_case} ; /* different commands require different parm1 validation */
    char *end_p = NULL ; 
    if (my_data->cmd_num==1) /* print_mat -> cmd_num==1 */
        type = print_case ;
    else if (my_data->cmd_num==5) /* mul_scalar -> cmd_num==5 */
        type = mul_scalar_case ;    
        else
            type = def_case ;
    switch(type) {
        case print_case: /* parm1 needs to be null */
            if (parm2!=NULL)
                return 3 ; /* Error: extra text */
            else
                return 0 ; 
            break ;
        case mul_scalar_case: /* parm2 needs to be a double */
            if (parm2==NULL)
                return 5 ; /* Error: missing argument */  
            scalar = strtod(parm2,&end_p) ;
            if (*end_p!='\0') /* there are chars that are not numbers */
                return 9; /* Error: argument is not a scalar */
            else
                my_data->scalar = scalar ; /* set scalar in my_data */
            break ;      
        case def_case: /* parm2 needs to be a mat */
            if (parm2==NULL)
                return 5 ; /* Error: missing argument */
            if (strlen(parm2)!=MAT_NAME_LEN)
                return 1 ; /* Error: undefined matrix name */
            mat_num = isValidMat(parm2) ;
            if (mat_num==-1)
                return 1 ; /* Error: undefined matrix name */
            else
                my_data->mat2 = &mats[mat_num] ; /* set mat1 in my_data */
            break ;       
    }
    return 0;
}

/* This function gets the 4th cut of the input and checks validation of #3 parmeter according to the command name 
    If valid - saves parm3 in my_data struct and returns 0.
    If invalid - returns error number 
    @param mat_num - matrix number in mats array 
    @param req - set to 0 if the command doesn't require a 3rd parameter and to 1 if requires. 
    @param parm3_required - enum to classify command types requirments. 
    @return int - the error number founded. */
int parm3Check(char *parm3, data *my_data, Mat *mats) {
    int mat_num, req ;
    enum parm3_required {yes,no} ; 
    if (my_data->cmd_num==1 || my_data->cmd_num==6 || my_data->cmd_num==7) /* commands that has no 3rd parm */
        req = no ;
    else /* commands that requires 3rd parm */
        req = yes ;
    switch(req){
        case yes: /* parm3 needs to be a mat */
            if (parm3==NULL) 
                return 5 ; /* Error: missing argument */
            if (strlen(parm3)!=MAT_NAME_LEN)
                return 1 ; /* Error: undefined matrix name */
            mat_num = isValidMat(parm3) ;
            if (mat_num==-1)
                return 1 ; /* Error: undefined matrix name */
            else {
                my_data->mat3 = &mats[mat_num] ; /* set mat3 in my_data */
                return 0; 
            }
            break;
        case no: /* parm3 needs to be null */
        if (parm3!=NULL)
            return 3; /* Error: extra text */           
    }
        return 0;
}
    
/*  This function checks validation of the read_mat command 
    The function goes over the command line, if the values are valid, they are saved in vals array in my_data struct 
    @param input_copy - copy of original input 
    @param jump -  number of chars on input until first charachter not part of command and mat name. 
    @param token - pointer to the current cut of the command line
    @param end_p - pointer to the rest of the string after seperating the numbers part. 
    @param i - index 
    @param counter - counts the number of values written in command line 
    @param num - contains the number written in the current cut of command line.
    @return int - the error number founded. */
int readCheck(data *my_data) {
    char *input_copy = my_data->input_copy ;
    size_t jump = strspn(input_copy, JUMP_OVER_CMD_MAT) ; /* calculate the jump required get to the numbers to be set */
    char *token = input_copy + jump ; /* cut command and mat  */
    char *end_P = NULL ;
    int i =0 , counter = 0 ;
    double num ;

    token = strtok(NULL,SPACE_COMMA_DEL) ; /* cut the #1 parm */
    if (token==NULL)
        return 0; /* it is legal to call read_mat with no parms */
    while(token!=NULL){
        num = strtod(token,&end_P); /* convret string to number */
        if (strcmp(end_P,"")!=0) /* chars that are not numbers */
            return 4 ; /* not a real number */
        /* real number */    
        counter++ ;
        if (i<SIZE*SIZE) /* less than 16 parms so far */    
            my_data->vals[i] = num ; /* set num in vals array */
        i++ ;
        token = strtok(NULL,SPACE_COMMA_DEL) ; /* cut the next parm */  
    }
    if (i<SIZE*SIZE) { /* if less than 16 parms, fill rest of vals[] with zeros. */
        while (i<SIZE*SIZE){
            my_data->vals[i] = 0 ;
            i++ ;
        }
    }
    return (read_commaCheck(my_data, counter)) ; /* call  read function comma check */

}

/* This functions searhes for a matching string in mats array. 
@param i - index 
@return int - matrix number if founded. -1 if not founded. */
int isValidMat(char *str) { 
    int i;
    for (i=0; i<NUM_OF_MATS; i++) {
        if (strcmp(str,mats[i]->mat_name)==0)
        return i ;
    }
    /* no match found */
    return (-1) ;
}




/*  This function checks if the comma apereances in the command line are valid. 
    This function is being called only when all seperate parts of the command line are valid 
    Single commas need to seperate parameters in the command line 
    Comma is illegal after command name 
    Multiple consecutive commas are illegal  
    @param comma_count - total numbers of commas in command line 
    @param consecutive_comma - counts the numbers of commas written in a row (with only spaces in between)
    @param input_copy - copy of original input 
    @param jump - number of chars on input until first charachter not part of command name 
    @param input - string of command line without command name
    @param cmd_num - current command number 
    @param len - rest of command line length. used in loop. 
    @param i - index
    @param comma_req - number of commas required according to the current command 
    @return int - the error number founded */
int commaCheck(data *my_data) {
    int comma_count = 0 ; /* comma appereance counter */
    int consecutive_comma = 0 ; /* consecutive comma counter */
    char *input_copy = my_data->input_copy ;
    size_t jump = strspn(input_copy,LETTERS_OF_CMD_AND_SPACE) ; /* number of chars that are part of command name */
    char *input = input_copy + jump ; /* jump over command name */
    int cmd_num = my_data->cmd_num ;
    int len = strlen(input) ;
    int i , comma_req ; /* num of commas required for each command type */
    
    if (*input==COMMA)
        return 6; /* Error: illegal comma */
    for (i=0; i<len; i++){
        if (*(input+i)==COMMA){ /* if current char is a comma, count.  */
            comma_count++;
            consecutive_comma++;
            continue ;
        }
        if (isspace(*(input+i))!=0) /* if current char is a space, move on. */
            continue ;
        else  /* current char is not a space and not a comma */
            consecutive_comma = 0 ; /* zero consecutive counter */   
    }
    if (consecutive_comma > 1)
        return 8 ; /* Error: multiple consecutiva commas error */

    /* set required number of commas */
    if (cmd_num == 1) /* print_mat */
        comma_req = 0 ;
    if (cmd_num == 6) /* tran_mat */
        comma_req = 1 ;
    if (cmd_num == 2 || cmd_num == 3 || cmd_num == 4 || cmd_num == 5) /* add,sub,mul,mul_scalar */
        comma_req = 2 ;

    if (comma_count < comma_req)
        return 7 ; /* Error: missing comma */
    if (comma_count > comma_req)
        return 6 ; /* Error: illegal comma */
    else
        return 0 ; /* no error */
    
}

/*  This function checks if the comma apereances in the read_mat command line are valid. 
    The funciton gets my_data struct and the number of parms counted in readCheck function. 
    This function is being called after command name and mat name were validated 
    Single commas need to seperate parameters in the command line 
    Comma is illegal after command name 
    Multiple consecutive commas are illegal 
    @param comma_count - total numbers of commas in command line 
    @param consecutive_comma - counts the numbers of commas written in a row (with only spaces in between)
    @param input_copy - copy of original input 
    @param jump - number of chars on input until first charachter not part of command name 
    @param input - string of command line without command name
    @param len - rest of command line length. used in loop. 
    @param i - index
    @return int - the error number founded*/
int read_commaCheck(data *my_data, int num_of_parms) {
    int comma_count = 0 ; /* comma appereance counter */
    int consecutive_comma = 0 ; /* consecutive comma counter */
    char *input_copy = my_data->input_copy ;
    size_t jump = strspn(input_copy,JUMP_OVER_READ) ; /* number of chars that are part of command name */
    char *input = input_copy + jump ; /* jump over command name */
    int len = strlen(input) ; 
    int i  ; 
    
    if (len==0)
        return 0 ; /* legal to call read without parms */
    if (*input==COMMA)
        return 6; /* illegal comma after command  */
    for (i=0; i<len; i++){
        if (*(input+i)==COMMA){ /* if current char is a comma, count it */
            comma_count++;
            consecutive_comma++;
            continue ;
        }
        if (isspace(*(input+i))!=0) /* if current char is a space, move on. */
            continue ;
        else  /* current char is not a space and not a comma */
            consecutive_comma = 0 ; /* zero consecutive counter */   
    }

    if (consecutive_comma > 1)
        return 8 ; /* Error: multiple consecutiva commas error */
    if (comma_count < num_of_parms)
        return 7 ; /* Error: missing comma */
    if (comma_count > num_of_parms)
        return 6 ; /* Error: illegal comma */

    /* comma_count == num_of_parms */
    return 0 ; /* no error */
}
    
    


