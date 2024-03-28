#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"


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
void errorHandler() {

}

/* creating a new mac_text struct and setting line in it's text member */
mac_text *createText (char *line) {
    mac_text *result = malloc(sizeof(mac_text)) ;
    result->text = malloc(sizeof(line)) ;
    strcpy(result->text,line) ;
    result->next = NULL ;
    return result ;
}

/* This function recieves a word and a number and creates a new Word struct with that word number and sets the next pointer to null */
macro *createMacro (char *name, char *line) {
    macro *result = malloc(sizeof(macro)) ;
    result->mac_name = malloc(sizeof(name)) ;
    strcpy(result->mac_name,name) ;
    result->next = NULL ;
    result->text = createText(line);
    return result ;
}

/* this functions get's a string and the head of the list, and searches for it in the macros list. if founded, it returns a pointer to it's text. else, null. */
macro *searchMcrList(char *word, macro *head) {
    macro *macP = head ;
    if (head == NULL) { /* list is empty */
        return NULL ;
    }
    while (macP != NULL){
        if (strcmp(word,macP->mac_name)==0) {
            return macP ;
        }
        macP = macP->next ;
    }
    return NULL ;
}

/* this functions copies a given macro content to the new file */
void copyMcrText(macro *cur_mac, FILE *newP) {
    mac_text *textP = cur_mac->text ;
    while (textP != NULL) {
        fputs(textP->text,newP) ;
        textP = textP->next ;
    }
}

/* this functiond checks if the macro name is valid. returns 1 for valid. 0 for invalid. */
int validateMcrName(char *mcr_name, macro *head) {
    int i ;
    /* check the cmd name is not a command name */
    for (i=0; i<NUM_OF_CMDS; i++) {
        if (strcmp(commands[i],mcr_name) == 0)
            return 0 ; 
    }
    /* check if mcr name was already defined */
    if (searchMcrList(mcr_name, head) != NULL)
        return 0; 
    return 1 ; /* valid name */
}

/* this functions adds a new macro defenition to the macro list. it returns when reaching endmcr or eof */
void addMcr(char *name, FILE *fp, macro **head) {
    char line[MAX_LINE_LEN] ; 
    macro *new_mac = NULL ;
    macro *macP = *head ;


    /* get the macro first line of content */
    fgets(line, MAX_LINE_LEN, fp) ;
    if (line == NULL) { /* reached eof */
        fprintf(stderr, "error: reached EOF before endmcr\n") ;
        exit(1) ;
    }
    new_mac = createMacro(name,line) ; /* create macro object with the first text line */
    /* connect the new nacro item to the list */
    if (*head==NULL) { /* if list is empty */
        *head = new_mac ;
    }
    /* go to end of list */
    else {
        while (macP->next != NULL) {
            macP = macP->next ;
        }
        macP->next = new_mac ; /* connect the new macro to the end of the list */    
    }
    
    /* if there are more command lines in the macro, add to list. */
    while ((fgets(line, MAX_LINE_LEN, fp)) != NULL ) {
        if (strcmp(line,"endmcr\n") == 0){ /* reched end of macro defenition */ 
            break ;
        }
        else    /* still in macro content */
            addText(new_mac, line) ;
    }
    
    return ;
    
}

void addText(macro *cur_mac, char *line) {
    mac_text *new_text = createText(line) ; /* create the new row node */
    mac_text *textP = cur_mac->text ;
    /* go to end of text list */
    while (textP->next != NULL ){
        textP = textP->next ;
    }
    textP->next = new_text ;

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



