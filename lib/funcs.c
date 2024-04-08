#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/assembler.h"


char *addFileEnding(char *file_name, int type) {
    char ending[MAX_ENDING_LEN] ;
    size_t file_name_len ;
    size_t ending_len ;
    char *result ;
    switch (type) {
        case 0: 
            strcpy(ending,".as") ;
            break ;
        case 1: 
            strcpy(ending,".am") ;
            break ;
        case 2: 
            strcpy(ending,".o") ;
            break ;
        case 3: 
            strcpy(ending,".externals") ;
            break ;
        case 4: 
            strcpy(ending,".entries") ;
            break ;
        default:
            return NULL; /* Return NULL for unsupported types */
    }

    file_name_len = strlen(file_name);
    ending_len = strlen(ending);

    /* Allocate memory for the new filename */
    result = malloc(file_name_len + ending_len + 1); /* Plus 1 for the null terminator */
    if (result == NULL) {
        return NULL; /* Memory allocation failed */
    }

    /* Copy the original filename and append the ending */
    strcpy(result, file_name);
    strcat(result, ending);

    return result;


}


/* this function gets a obj file pointer and a binary string. it tranlates the binary code to encrypted 4 base code and writes the line in the obj file */
void binToFour(FILE *obj_fp, char *str) {
    int i ;
        for (i=0; i<BIN_WORD_LEN; i=i+2) {
            if (*(str+i) == '0') {
                if (*(str+i+1)=='0') { /* 00 */
                    fputc('*',obj_fp) ;
                    continue ;
                }
                else {/* 01 */
                    fputc('#',obj_fp) ;
                    continue ;
                }
            }
            else {
                if (*(str+i+1)=='0') { /* 10 */
                    fputc('%',obj_fp) ;
                    continue ;
                }
                else {/* 11 */
                    fputc('!',obj_fp) ;
                    continue ;
                }
            }
        }
    fputc('\n',obj_fp) ;
    str++ ; 
}

/* 
*   This function checks to see if a targetWord exists in a word_array
*   Returns 1 if it is, 0 if it isnt.
*/
int checkWordInArray(char **word_array, char *targetWord){
    int i;
    for (i = 0; word_array[i] != NULL; i++){
        if (strcmp(word_array[i], targetWord) == 0){
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
*   Opens a file with a specific ending.
*/
FILE *openFile(char *clean_file_name, int num) {
    char *file_name = NULL;
    char *temp_name = NULL; 
    FILE *file = NULL; 
    temp_name = addFileEnding(clean_file_name,num);
    file_name = malloc(strlen(temp_name)+1);
    check_allocation(file_name);
    strcpy(file_name,temp_name);
    free(temp_name);
    file = fopen(file_name, "r"); 
    check_allocation(file);
    return file;
}
