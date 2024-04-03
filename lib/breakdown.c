#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/assembler.h"

void scan_file(FILE *file){
    char line[MAX_LINE_LEN];
    char *inputCopy = NULL;
    char *pointer = NULL;
    Label_Type label_type;
    while (1) {
        if (fgets(line, MAX_LINE_LEN, file) == NULL) {
            if (feof(file)) {
                break;
            }
        }
        if (strlen(line) == MAX_LINE_LEN - 1 && line[MAX_LINE_LEN - 2] != '\n') {           /* Checking to see if the array is full without \n */
            errorCode = ERR_SIZE_LEAK;
            error_manager(errorCode);
        }
        else {
            inputCopy = (char *)malloc(strlen(line) + 1); 
            check_allocation(inputCopy);
            strcpy(inputCopy, line); 

            pointer = strtok(inputCopy, " \t\n");                                             /* Get the first word from the line */
            printf("Line: %s",inputCopy);

            switch (check_first_word(pointer)){
                case 0:                                                                      
                    break;                                                                  /* Blank or note line - ignore */ 
                case 1:                                                                     
                    /* .define .extern (entries not right now!!)*/
                    label_type = getLabelType(pointer);  
                    /* define sz=2
                    1) is it extern
                        if no -> it is a define...go to step 2
                        if yes -> go to step 2.5
                    2) strtok(" =\t\n")
                    2.5) strtok(" \t\n")
                    3) check if it is a valid label name (1 <= label <= 31) ######
                        if yes -> go to step 4
                        if no -> return error, go to step 7
                    3) check if label exists in label tabel
                        if yes -> return error, go to step 7
                        if no -> go to step 4
                    4) is it extern? 
                        if yes -> add to label tabel, go to step 7
                        if no -> go to step 5
                    5) go to next word in the line strtok(" \t\n")
                        confirm received an int that can fit into 14 binary
                    6) check that the next word is a NULL (no extra text)
                        if yes -> return error, go to step 7
                        if no, go to step 7
                    7) free() everything and next line

                    ######
                        all letters are alphabetic characters (small or big)
	                    maximum number is 31 characters (if define or extern or entry, 1<=label<=31) (if data string or cmd label, 1<label<=32)
	                    can not be a register / other name already in the program
                    ######

                    ######
                        can use a manipulation of check_read_mat from maman22 for counting the commas and dealing with the numbers
                        after every comma: + or - or nothing (nothing means +). (use atoi manipulation )
                        after every +/- there is no whitespace!
                        ask gpt: if i have a string such as "-57" is there a C function that can automaticaly check and transfer it to an int as -57 ?"
                    ######
                    */     
                                 
                    curr_line_number++;
                    break;
                case 2:                                                                     /* potential command / data / string label */
                    /* 
                    1) check if first word is a correct label (1 < label <= 32)  ######
                        if no, return error, go to step 8
                        if yes, go to step 2.
                    2) check if the label exists in the label tabel.
                        if yes -> return error, go to step 8 (no need further checks because no entries yet!!)
                        if no -> continue to step 3
                    3) check if second word is a command
                        if yes, add to label tabel with name,cmd_label,address=IC
                            send the remainder to mihal
                        if no, go to step 4
                    4) check if second word is .data or .string
                        if no, return error, go to step 8
                        if yes, go to step 5
                    5) get the remainder of the line. 
                        if its string -> check if starts with "
                            if no, return error invalid string go to step 8
                            if yes, check if ends with "
                                if no, check if there is a " inside the array.
                                    if yes, return error extra_txt
                                    if no, return error invalid string go to step 8
                                if yes, create new node
                                    go through the string from second char (after ") all the way to the end (when ") 
                                    and create new data field for every value in ascii.
                                    go to step 6.
                        if its a data -> use check_read_mat from maman22 to extract the array while counting the commas
                            create a new node and new data field for every value.
                            after every comma: + or - or nothing (nothing means +). (use atoi manipulation) #######
                                after every +/- there is no whitespace!
                            go to step 6.
                    6) add the label to label tabel with the correct data. address is current DC.
                        after adding, increment DC+L
                    7) check no remainding text
                    8) free() everything next line

                     */

                    curr_line_number++;
                    break;
                case 3: /* Everything else */

                    curr_line_number++;
                    break;
                default:
                    break;
            }
        }
        free(inputCopy); 
    }
}


