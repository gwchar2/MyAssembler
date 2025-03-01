#include "../include/assembler.h"

/* 
* This function creates a new mac_text node 
*   @param – line – a string to add to the current macro
*   @return – result – the new mac_text created
*/

mac_text *createText (char *line) {
    mac_text *result = malloc(sizeof(mac_text));
    result->text = malloc(sizeof(line));
    check_allocation(result);
    check_allocation(result->text);
    strcpy(result->text,line);
    result->next = NULL;
    return result;
}

/* 
* This function creates a new macro node
*   @param – name – a string containing the new macro name
*   @param - line – a string containing the first line of contents in this macro 
*   @return – result – the new macro node created
*/
macro *createMacro (char *name, char *line) {
    macro *result = malloc(sizeof(macro));
    result->mac_name = malloc(sizeof(name));
    check_allocation(result);
    check_allocation(result->mac_name);
    strcpy(result->mac_name,name);
    result->next = NULL;
    result->text = createText(line);
    return result;
}

/* 
* This function searches a string in the macro’s list
*   @param – word – a string to search
*   @param – mcr_head – the head of the macro’s list 
*   @return – if found – pointer to the macro founded. else – null
*/
macro *searchMcrList(char *word, macro *mcr_head) {
    macro *macP = mcr_head;
    if (mcr_head == NULL) { /* list is empty */
        return NULL;
    }
    while (macP != NULL){
        if (strcmp(word,macP->mac_name)==0) {
            return macP;
        }
        macP = macP->next;
    }
    return NULL;
}

/* this functions copies a given macro content to the new file */
void copyMcrText(macro *cur_mac, FILE *newP) {
    mac_text *textP = cur_mac->text;
    while (textP != NULL) {
        fputs(textP->text,newP);
        textP = textP->next;
    }
}

/* 
* This function checks if a given name is a valid macro name
*   @param – mcr_name – a string to check
*   @param – mcr_head – pointer to the head of macro’s list
*   @return – 1 if valid. 0 if invalid
*/
int validateMcrName(char *mcr_name, macro *mcr_head) {
    int i;
    /* check the cmd name is not a command name */
    for (i=0; i<NUM_OF_CMDS; i++) {
        if (strcmp(commands[i],mcr_name) == 0)
            return 0; 
    }
    /* check if mcr name was already defined */
    if (searchMcrList(mcr_name, mcr_head) != NULL)
        return 0; 
    return 1; /* valid name */
}

/* 
* This function adds a new macro node to the macro’s list with all the text included
*   @param – name – the macro name
*   @param -fp – file pointer to the .as file from which the macro content is read
*   @param – mcr_head – pointer to the head of macro’s list
*   @return – when reacing endmcr or eof 
*/
void addMcr(char *name, FILE *fp, macro **mcr_head) {
    char line[MAX_LINE_LEN]; 
    macro *new_mac = NULL;
    macro *macP = *mcr_head;


    /* get the macro first line of content */
    fgets(line, MAX_LINE_LEN, fp);
    check_allocation(line);
    new_mac = createMacro(name,line); /* create macro object with the first text line */
    /* connect the new nacro item to the list */
    if (*mcr_head==NULL) { /* if list is empty */
        *mcr_head = new_mac;
    }
    /* go to end of list */
    else {
        while (macP->next != NULL) {
            macP = macP->next;
        }
        macP->next = new_mac; /* connect the new macro to the end of the list */    
    }
    
    /* if there are more command lines in the macro, add to list. */
    while ((fgets(line, MAX_LINE_LEN, fp)) != NULL ) {
        if (strcmp(line,"endmcr\n") == 0){ /* reched end of macro defenition */ 
            break;
        }
        else    /* still in macro content */
            addText(new_mac, line);
    }
    
    return;
    
}

/* 
*   This function adds the next line text to the macro’s text list
*   @param – cur_mac – pointer to the current macro being defined
*   @param – line – a string containing the next line in the macro
*/
void addText(macro *cur_mac, char *line) {
    mac_text *new_text = createText(line); /* create the new row node */
    mac_text *textP = cur_mac->text;
    /* go to end of text list */
    while (textP->next != NULL ){
        textP = textP->next;
    }
    textP->next = new_text;

}

