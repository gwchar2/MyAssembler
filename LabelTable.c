#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"


/*
* Extracts a line and reviews it.
* @Params:
* file - file pointer to read lines from
* label_node **head - first label in tavlat smalim
* label_node **tail - last label in tavlat smalim
*/
void make_table(FILE *file,label_node **head,label_node **tail){
    /* Reads lines from the file and handles them */
    char line[MAX_LINE_LEN];
    int curr_line_number = 100;
    while (1) {
        /* Read a line with the size of MAX_LINE_LEN */ 
        if (fgets(line, MAX_LINE_LEN, fp) == NULL) {
            if (feof(file)) {                                         /* If we are at the end of the file */
                break;                                                /* Terminate loop */
            } 
        }
        analyze_line(line,curr_line_number,head);

        /* Move to the next line, and line_number ++ */
        curr_line_number++;
    }
}

/*
*   This goes through the line and analyzes the words accordingly.
* @Params:
* char* line - the line to go through
* curr_line_number - the current address of the line (starts from 100)
* label_node **head - first label in tavlat smalim
* label_node **tail - last label in tavlat smalim
*/
void analyze_line(char* line,int curr_line_number,label_node **head,label_node **tail){
    char curr_line[MAX_LINE_LEN];
    strcpy(curr_line, line);                        /* Copy the current line */
    if (check_label(curr_line) != 1){                 /* If the word is a label */

    }
    return; /* Do i need return? */
}

/* 
* Analyzes the first word in the row, and checks to see if it is a ';' (comment) or '\n' (empty row) or a cmd
* Returns 1 if it is one of the above, 0 else.
*/
int check_label(char* curr_line){
    char *word = NULL;
    word = strtok(curr_line, (" \t"));
    if (word[0] == '\n' || word[0] == ';' || (word_is_cmd(word) == 1))                      /* Checks to see if first character is \n or ; so it can ignore them*/
        return 1;
    else 
        return 0;                                              /* Else, word is a label! */
}

/*
* This function checks to see if a word symbolizes a command
* If the word is equal to one of the initiating words for a command in a program, return 1
* Else, return 0.
*/
int word_is_cmd(char *word){
    int i = 0;
    for (i = 0; i < NUM_OF_CMDS; i++) { 
        if (strcmp(commands[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}




/*
* צריך לעבור על המילה ולכתלג סוגים של לייבלים 
*  אם זה דיפיין או דברים אחרים
* ליצור את הנוד המתאים ללייבל
* במהלך הסריקה אם נתקלים אז צריך לסרוק בטבלת הסמלים
*/




/* .entry LABEL or .extern LABEL or .define LABEL*/
/* 
Label : 
	- starts with an alphabetic letter
	- after it , alphabetic characters (small or big)
	- maximum number is 31 characters
	- after the 31, or at the end, must include :
	- does not appear more than once
	- can not be a register / other name already in the program
hEllo: .string ~~ OR .data ~~ OR command
אסור להגדיר תווית בשורה שהיא משפט הגדרת קבוע.
*/



/* in function word_is_cmd - need to than go through the command parsing and figure out if its using a label or not !*/