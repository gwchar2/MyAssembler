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
    printf("in bin translate\n") ; /* test use */
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
*/
int checkWordInArray(char **word_array, char *targetWord){
    int i,len;
    len = strlen(*word_array);
    for (i = 0; i < len ; i++){
        if (strcmp(word_array[i],targetWord) == 0){
            return 1;
        }
    }   
    return 0;
}

