#include "../include/assembler.h"



/*
*   This function translates the instruction segment data to binary value.
*/
void translateToBin(){
    cmd_node *cmd_temp = cmd_head;
    reg_node *reg_temp = NULL;
    char binary_word[BIN_WORD_LEN+1];

    while (cmd_temp != NULL){
        if (cmd_temp ->addressStyleRS >= 0 && cmd_temp -> addressStyleRT >= 0)
            strcpy(cmd_temp -> bin_value,cmdBinTranslation(cmd_temp -> instruction,cmd_temp ->addressStyleRS, cmd_temp -> addressStyleRT));
        reg_temp = cmd_temp -> next_reg;
        while (reg_temp != NULL){
            /* If both the arguments received are registers, we combine them to 1 14 bit word */
            if (cmd_temp -> addressStyleRS == 3 && cmd_temp -> addressStyleRT == 3){
                strcpy(reg_temp -> bin_value,RSBinTranslation(reg_temp -> data));
                strcpy(reg_temp -> next_reg -> bin_value,RTBinTranslation(reg_temp -> next_reg -> data));
                strcpy(binary_word,combineRegBin(reg_temp -> bin_value,reg_temp -> next_reg -> bin_value));
                strcpy(reg_temp -> bin_value,binary_word);
                free(reg_temp->next_reg);
                reg_temp -> next_reg = NULL;
            }else {
                /* If its a reg, we put the value of the reg in the RS / RT location */
                if (reg_temp -> reg_type == REG){
                    if (reg_temp -> RS == 1 )  
                        strcpy(reg_temp -> bin_value,RSBinTranslation(reg_temp -> data));
                    else if (reg_temp -> RT == 1 )  
                        strcpy(reg_temp -> bin_value,RTBinTranslation(reg_temp -> data));
                }
                else {
                    strcpy(reg_temp -> bin_value, BinTranslation12Bit(reg_temp -> data, reg_temp -> ARE));
                }
            }
            reg_temp = reg_temp -> next_reg;
        }
        cmd_temp = cmd_temp -> next_cmd;
    }
}


/*  
* This function translate a command data to the command line binary representation 
* according to the command number and the operands’ addressing methods.
*   @param - cmd_num – the command opcode as int
*   @param – sourceAdd – the source operand addressing method
*   @param – targetAdd – the target operand addressing method 
*/
char *cmdBinTranslation(int cmd_num, int sourceAdd, int targetAdd) {
    static char result[BIN_WORD_LEN+1];
    int i,j;
    char opcode[OPCODE_BIN_LEN+1];
    /* left 4 bits are always set to 0 */
    for (i=0; i<=3; i++) {
        result[i] = '0';
    }

    /* set opcode */
    strcpy(opcode,opcodeBinTranslation(cmd_num));

    for (i=OPCODE_FIELD, j=0; i<=OPCODE_FIELD+OPCODE_BIN_LEN; i++, j++) {
        result[i] = opcode[j];
    }
    
    /* set source op addressing method in bits [5-4]*/
    switch (sourceAdd) {
        case 0:
            result[RSA_FIELD] = '0';
            result[RSA_FIELD+1] = '0';
            break;
        case 1:
            result[RSA_FIELD] = '0';
            result[RSA_FIELD+1] = '1';
            break;
        case 2:
            result[RSA_FIELD] = '1';
            result[RSA_FIELD+1] = '0';
            break;
        case 3:
            result[RSA_FIELD] = '1';
            result[RSA_FIELD+1] = '1';
            break;
    }
    /* set target op addressing method in bits [3-2]*/
    switch (targetAdd) {
        case 0:
            result[RTA_FIELD] = '0';
            result[RTA_FIELD+1] = '0';
            break;
        case 1:
            result[RTA_FIELD] = '0';
            result[RTA_FIELD+1] = '1';
            break;
        case 2:
            result[RTA_FIELD] = '1';
            result[RTA_FIELD+1] = '0';
            break;
        case 3:
            result[RTA_FIELD] = '1';
            result[RTA_FIELD+1] = '1';
            break;
    }
    /* right 2 bits are always set to 0 */
    result[BIN_WORD_LEN-1] = '0';
    result[BIN_WORD_LEN - 2] = '0';
    result[BIN_WORD_LEN] = '\0'; 
    return result; 
}


/*  
* This function translates an integer to a 4 bit binary representation as a string
* 4 bits to fit the opcode part of the command binary instruction.
*   @param – num – the number to translate 
*   @return – result – the binary string 
*/
char *opcodeBinTranslation(int num) {
    static char result[OPCODE_BIN_LEN +1]; 
    int bit, i;
    int index = 0;
    
    for (i = OPCODE_BIN_LEN-1; i >= 0; i--) { 
        bit = (num >> i) & 1;
        result[index] = bit + '0'; 
        index++;
    }
    result[index] = '\0'; 
    return result; 
}

/*  
* This function translates a number to a 12 bit binary representation. 
* The function adds the ARE to the last 2 bits.
*   @param – num – the number to translate
*   @param – ARE – the matching ARE code
*   @return -result –  14 binary bits as a string 
*/
char *BinTranslation12Bit(int num, int ARE) {
    static char result[BIN_WORD_LEN+1];
    int i, bit, index = 0;
    int negHandle = 1 << BITS_IN_INT; /* for neg numbers, add 4096 to get the correct Two's complement bin representation */
    /* Handle negative numbers using Two's complement */
    if (num < 0) {
        num += negHandle;
    }
    /* Store the binary representation of num in the string */
    for (i = BITS_IN_INT - 1; i >= 0; i--) {
        bit = (num >> i) & 1;
        result[index] = bit + '0'; 
        index++;
    }
    switch (ARE) {
        case 0:
            /* set the ARE fIEld to 00 */
            result[LSB-1] = '0';
            result[LSB] = '0';
            result[LSB+1] = '\0'; 
            break;
        case 1:
            /* set the ARE fIEld to 01 */
            result[LSB-1] = '0';
            result[LSB] = '1';
            result[LSB+1] = '\0'; 
            break;
        case 2:
            /* set the ARE fIEld to 10 */
            result[LSB-1] = '1';
            result[LSB] = '0';
            result[LSB+1] = '\0'; 
            break;

    }
    return result;
}

/*  
* This function translates a number to it’s 14 bit binary representation.
*   @param – num – the number to translate.
*   @return  - result – the translated string 
*/
char *BinTranslation14Bit(int num) {
    static char result[BIN_WORD_LEN+1];
    int i, bit, index = 0;
    int negHandle = 1 << BIN_WORD_LEN; 

    /* Handle negative numbers using Two's complement */
    if (num < 0) {
        num += negHandle;
    }

    /* Store the binary representation of num in the string */
    for (i = BIN_WORD_LEN-1; i >= 0; i--) {
        bit = (num >> i) & 1;
        result[index] = bit + '0'; 
        index++;
    }
    return result;
}

/*  
* This function gets RS number, translate it to 3 bits binary representation and place it in the 
* RS bit field.
*   @param – reg_num – the register number to translate
*   @return – 14 bits binary string
*/ 
char *RSBinTranslation(int reg_num) {
    static char result[BIN_WORD_LEN+1];
    int num = reg_num << RS_SHIFT; /* shifting left to get the RS number in the 5-7 bits */
    strcpy(result, BinTranslation12Bit(num,0)); /* use imm translation func */  
    return result;
}


/*  
* This function gets RT number, translate it to 3 bits binary representation and place it in the 
* RT bit field.
*   @param – reg_num – the register number to translate
*   @return – 14 bits binary string
*/ 
char *RTBinTranslation(int reg_num) {
    static char result[BIN_WORD_LEN+1];
    strcpy(result, BinTranslation12Bit(reg_num,0)); /* use imm translation func */  
    return result;
}

/*  
* In case both operands are registers, they should share a binary string.
* This function combines both registers values in one 14 bits binary string.
*   @param - str1 – RS separated binary string
*   @param – str2 – RT separated binary string
*   @return – result – combined 14 bits binary string
*/
char *combineRegBin(char *str1, char *str2) {
    int i;
    static char result[BIN_WORD_LEN+1];
    for (i=0; i<=BIN_WORD_LEN; i++) {
        if (*(str1+i) == '1' || *(str2+i) == '1')
            result[i] = '1';
        else
            result[i] = '0';
    }
    result[BIN_WORD_LEN] = '\0';
    return result;

}


/* 
* This functions translates a binary string to the encrypted 4 base code string.
*   @param – obj_fp – a file pointer to the final .o file
*   @param – str – binary string to translate
*/
void binToFour(FILE *obj_fp, char *str) {
    int i;
        for (i=0; i<BIN_WORD_LEN; i=i+2) {
            if (*(str+i) == '0') {
                if (*(str+i+1)=='0') { /* 00 */
                    fputc('*',obj_fp);
                    continue;
                }
                else {/* 01 */
                    fputc('#',obj_fp);
                    continue;
                }
            }
            else {
                if (*(str+i+1)=='0') { /* 10 */
                    fputc('%',obj_fp);
                    continue;
                }
                else {/* 11 */
                    fputc('!',obj_fp);
                    continue;
                }
            }
        }
    fputc('\n',obj_fp);
    str++; 
}

