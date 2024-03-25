#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"


char line[MAX_LINE_LEN] ;
char *token = NULL ;
int macro_flag = 0;
macro *head ; 


while((fgets(line, MAX_LINE_LEN, fp)) != NULL) {
    token = strtok(line," \n\t");
    if (strcmp(token,"mcr")){
        macro_flag = 1 ;
        token = strtok(NULL," \n\t") ; /* cut macro name */
        addMcr(token, fp); /* copy macro content to macro list */

    }


}

void addMcr(char *name, FILE *fp) {
    char line[MAX_LINE_LEN] ; 
    mac_text *new_text = malloc(sizeof(mac_text)) ;
    macro *new_mac = malloc(sizeof(macro)) ;
    strcpy(new_mac->mac_name,name) ;
    (fgets(line, MAX_LINE_LEN, fp)) ;
    strcpy(macro->mac_text->text, line) ;
    while((fgets(line, MAX_LINE_LEN, fp)) != "endmcr") {
        
    }
}