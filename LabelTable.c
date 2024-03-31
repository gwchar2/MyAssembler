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

