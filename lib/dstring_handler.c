#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "../include/assembler.h"

/*
* Returns 0 if error, 1 if ok, 2 if needs cmd check
*/
int dstring_handler(char *pointer){
    char *label_name = NULL;                                            /* Copies the pointer */
    char *p_copy = NULL;
    int i;
    Label_Type label_type;
    label_node *temp_node = NULL;
    label_name = pointer;
    i = 0;
    while (pointer[i] != ':'){
        i++;
    }
    pointer[i] = '\0';
    label_name = malloc(strlen(pointer));
    check_allocation(label_name);
    strcpy(label_name,pointer);
    pointer = strtok(NULL," \t\n\r\f\v");                              /* Increments the copy (pointer stays the same as before in father func) */
    /* Check to see if missing argument */
    if (pointer == NULL){
        errorCode = ERR_UNDEFINED_ARGUMENT;
        error_manager(errorCode);
        return 0;
    }
    
    else{
        label_type = getLabelType(pointer);
        /* Check the label */
        if (!check_label(label_name,label_type)){
            return 0;
        }
        /* If LABEL: .entry or LABEL: .extern we ignore */
        if (strcmp(pointer,".entry") == 0 || strcmp(pointer,".extern") == 0){
            return 0;
        }

        /* Check the remainding text */
        pointer = strtok(NULL,"\n\r\f\v"); 

        if (pointer == NULL){
            errorCode = ERR_UNDEFINED_ARGUMENT;
            error_manager(errorCode);
            return 0;
        }
    
        /* Fetch data according to label type */
        if (label_type == STRING_LABEL || label_type == DATA_LABEL){
            p_copy = malloc(strlen(pointer));
            check_allocation(p_copy);
            strcpy(p_copy,pointer);

            /* CHECK THE DATA */
            if (!check_data(p_copy,label_type)){
                return 0;
            }
            add_label(DC,DC,label_name,0,label_type);
            /* search & grab the new node */
            temp_node = label_exists(label_name);
            if (DC == 1){
                dc_head = label_exists(label_name);
            }


            /* Fetch data according to label type */
            if (label_type == STRING_LABEL){
                fetch_data(p_copy,temp_node);
                DC = DC + (temp_node -> data_count);
                return 1;                
            }
            else if (label_type == DATA_LABEL){
                fetch_data(p_copy,temp_node);
                DC = DC + (temp_node -> data_count);
                return 1;
            }

        }
        else if (label_type == CMD_LABEL){
            add_label(IC,0,label_name,0,label_type);
            return 2;
        }
    }
    return 1;
}

int check_data(char *p_copy,Label_Type label_type){
    /* Copy the string for safety */
    int i,counter,len,total_comma,total_comma_in_a_row,var_counter,pos,neg,flag;
    char *string = NULL;
    label_node *temp = NULL;
    counter = 0;
    var_counter = 0;
    string = malloc(strlen(p_copy));
    check_allocation(string);
    strcpy(string,p_copy);

    /* Check the string according to label type */
    if (label_type == STRING_LABEL){
        /* As long as last char is useless, decrease the string */
        while (string[strlen(string)-1] == '\n' || string[strlen(string)-1] == '\t' || string[strlen(string)-1] == ' '){
            string[strlen(string)-1] ='\0';
        }

        /* If string is missing parentheses  */
        if (string[0] != '"' && string[strlen(string)-1] != '"'){
            errorCode = ERR_MISSING_PARENTHESES;
            error_manager(errorCode);
            return 0;
        }

        /* If string is: "  */
        else if ((string[0] == '"' && string[strlen(string)-1] != '"')){
            len = strlen(string);
            for (i = 0; i < len; i++){
                if (string[i] == '"'){
                    counter++;
                }
            }
            if (counter >= 2){
                errorCode = ERR_EXTRANEOUS_TEXT;
                error_manager(errorCode);
                return 0;
            }
        }

        /* If string is:    " */
        else if ((string[0] != '"' && string[strlen(string)-1] == '"')){
            len = strlen(string);
            for (i = 0; i < len; i++){
                if (string[i] == '"'){
                    counter++;
                }
                if (counter >= 2){
                errorCode = ERR_UNDEFINED_ARGUMENT;
                error_manager(errorCode);
                return 0;
                }
            }
        }
    }
    else if (label_type == DATA_LABEL){
        int numCounter = 0, total_comma = 0, total_comma_in_a_row = 0,j; 
        size_t i = 0, size_remainder = strlen(string);
        char *pointer = NULL;
        string = malloc(strlen(p_copy));
        check_allocation(string);
        strcpy(string,p_copy);
        pointer = strtok(string," ,\t\n\r\f\v");

        /* As long as we have not reached the end of the input */
        while (pointer != NULL){     

            /* If the current pointer string is all alphabetical letters, it must be a define! */                    
            if (check_alpha(pointer)) {
                temp = label_exists(pointer);

                /* If no such define exists, or if a label exists but its NOT a define */
                if (temp == NULL || ((temp -> label_type) != DEF_LABEL)){
                    errorCode = ERR_UNDEFINED_ARGUMENT;
                    error_manager(errorCode);
                    return 0;
                }
            }
            /* If its not fully alphabetical, than it must be a number! RIGHT? So lets check the value! if its OOVVEERR 9000! (meme) its no good... */
            else if (strToInt(pointer) == 9000){
                errorCode = ERR_UNDEFINED_ARGUMENT;
                error_manager(errorCode);
                return 0;
            }
            numCounter++;
            pointer = strtok(NULL," ,\t\n\r\f\v");
        } 
        /* The maximum amount of numbers is 36 (with 1 letter label & 35 commas) */
        if (p_copy != NULL && numCounter >= 36){                                                   
            errorCode = ERR_EXTRANEOUS_TEXT;
            error_manager(errorCode);
            return 0;
        }

        /* Checks for extra and consecutive commas */
        for (i = 0; i < size_remainder; i++){
            if (!WHITESPACE(p_copy[i])){
                if (p_copy[i] == ','){
                    total_comma++;
                    total_comma_in_a_row++;
                    if (total_comma_in_a_row >= 2){
                        errorCode = ERR_MULTIPLE_CONSECUTIVE_COMMAS;
                        error_manager(errorCode);
                        return 0;
                    }
                }
                else {
                    total_comma_in_a_row = 0;
                } 
            }
        }
        /* These statements return errors according to what we previously found out */
        if ((numCounter == 0 && total_comma >= 1) || total_comma > 35 ){
            errorCode = ERR_ILLEGAL_COMMA;
            error_manager(errorCode);
            return 0;
        }
        else if (total_comma < numCounter-1){
            errorCode = ERR_MISSING_COMMA;
            error_manager(errorCode);
            return 0; 
        }
        else if (total_comma >= numCounter){
            errorCode = ERR_EXTRANEOUS_TEXT;
            error_manager(errorCode);
            return 0;
        }
    }
    free(string);
    return 1;
}

void fetch_data(char *p_copy, label_node *temp_node){

    /* Copy the string for safety */
    int i,len;
    char *string = NULL;
    char *pointer = NULL;
    char *num;
    label_node *ptr;
    string = malloc(strlen(p_copy));
    check_allocation(string);
    strcpy(string,p_copy);
    
    /* We now fetch according to the label type */
    /* For string, just go through the string and translate to ASCII */
    if (temp_node -> label_type == STRING_LABEL){
        len = strlen(string)-2;
        string = string+1;
        for (i = 0; i < len; i++){
            add_data(string[i],temp_node);
        }
        add_data(0,temp_node);
    }

    /* If its a data label */
    else if (temp_node -> label_type == DATA_LABEL){
        pointer = strtok(string," ,\t\n\r\f\v");
        
        /* Go through the data array that we received */
        while (pointer != NULL){
            num = malloc(strlen(pointer));
            check_allocation(num);
            strcpy(num,pointer);

            /* If we find a pointer that is not a number .. it must be a define label, since we already checked !!! */
            if (strToInt(num) == 9000){
                ptr = label_exists(num);

                /* Add the defined Data */
                if (ptr != NULL){
                    add_data(ptr -> definedData,temp_node);
                }
            }
            else{
                add_data(strToInt(num),temp_node);
            }
            pointer = strtok(NULL," ,\t\n\r\f\v");
        }
    }
}
