#ifndef HEADER_H
#define HEADER_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
/******************************* 
****** DEFINES AND MACROS ******
********************************/
#define NUM_OF_CMDS 16
#define MIN_ARGV 2
#define FILE_NAME_LEN 50
#define MAX_LINE_LEN 80
#define MAX_ENDING_LEN 11
#define BIN_WORD_LEN 14
#define NUM_OF_REGS 10
#define MAX_LINE_LEN 80
#define FIRST_GROUP_VARS 2
#define SECOMD_GROUP_VARS 1
#define THIRD_GROUP_VARS 0
#define COMMA ','
#define SPACE_COMMA_DEL ", \f\r\t\v\n"
#define BITS_IN_INT 12
#define MAX_12BITS 2047
#define MIN_12BITS -2048
#define BIN_WORD_LEN 15
#define RS_FIELD 5







/* Checks if the memory for (C) was allocated properly */
#define check_allocation(c)\
        if (c == NULL){\
            fprintf(stderr, "Error allocating memory %s",strerror(errno));\
            exit(EXIT_FAILURE);\
        }

/********************************* 
****** GLOBAL INTS & ENUMS *******
**********************************/
typedef enum ErrorCode{
    ERR_UNDEFINED_REGISTER,
    ERR_UNDEFINED_COMMAND,
    ERR_EXTRANEOUS_TEXT,
    ERR_UNDEFINED_ARGUMENT,
    ERR_MISSING_ARGUMENT,
    ERR_ILLEGAL_COMMA,
    ERR_MISSING_COMMA,
    ERR_MULTIPLE_CONSECUTIVE_COMMAS,
    ERR_SEGMENTATION_FAULT,
    ERR_REDEFINITION_MACRO
    ERR_ILLEGAL_ADDRESSING,
} ErrorCode; 

typedef enum Label_Type{
    CMD_LABEL,
    DATA_LABEL, /* Includes string, but if it is string, it must have \0 as last data array (14 zeros)*/
    ENTRY_LABEL,
    EXTERNAL_LABEL
} Label_Type;

enum ending_type {
        as, 
        am,
        o,
        external, 
        entreis
};

extern char *commands[];
extern char *registers[];
extern int curr_line_number;
extern int IC;
extern int DC;
extern int err_flag;
extern ErrorCode errorCode;

/************************************
********** QUALITY OF LIFE **********
*************************************/
int checkWordInArray(char *words, char* targetWord); /* searches if a targetWord exists in an array of words*/

char *addFileEnding(char *file_name, int type); /* Adds the appropriate file ending */

void binToFour(FILE *obj_fp, char *str); /* Translates a string of 14 binary characters, to encrypted base 4 */
/************************************
********** MACRO STRUCTURE ********
*************************************/
typedef struct mac_text {
    char *text;
    struct mac_text *next;
} mac_text;

typedef struct macro {
    char *mac_name;
    struct mac_text *text;
    struct macro *next;
} macro;
extern macro *head;

mac_text *createText (char *line); /* creating a new mac_text struct and setting line in it's text member */

macro *createMacro (char *name, char *line); /* This function recieves a word and a number and creates a new Word struct with that word number and sets the next pointer to null */

macro *searchMcrList(char *word, macro *head); /* this functions get's a string and the head of the list, and searches for it in the macros list. if founded, it returns a pointer to it's text. else, null. */

void copyMcrText(macro *cur_mac, FILE *newP); /* this functions copies a given macro content to the new file */

int validateMcrName(char *mcr_name, macro *head); /* this functiond checks if the macro name is valid. returns 1 for valid. 0 for invalid. */

void addMcr(char *name, FILE *fp, macro **head); /* this functions adds a new macro defenition to the macro list. it returns when reaching endmcr or eof */

void addText(macro *cur_mac, char *line);

/************************************
********** COMMAND STRUCTURE ********
*************************************/
typedef struct Cmd_node{
    int address; /* The instruction count */
    int total_vars; /* The total amount of variables it holds */
    int L; /* num of bin words */
    char *binary_cmd;
    int sourceAdd;
    int targetAdd;
    char *source1_binary;
    char *source2_binary;
    char *target1_binary;
    char *target2_binary; 
    struct Cmd_node *next_cmd; /* Next cmd */
    struct Label_node *next_label; /* Next label (null until merging with DC ) */
} cmd_node;

extern cmd_node *cmd_head; /* Instruction segment head */
/************************************
********** LABEL STRUCTURE **********
*************************************/
typedef struct Label_node{
    int line_init; /* Address the label was intialized in */
    char *label_name; /* Label name */
    int data_count; /* Total amount of data stored in the label */
    enum Label_Type label_type; /* Type of label */

    struct Bin_data *data; /* The data list */
    struct Row_node *row_list; /* The row list (the rows which it appears in )*/
    struct Label_node *next_label; /* Next label */
    struct Label_node *prev_label; /* Prev label */
} label_node;

typedef struct Bin_data{ /* For string, we put in ascii values */
    char string[15];
    struct Bin_data *next_data;
} bin_data;

typedef struct Row_node{
    int address;
    struct Row_node *next_row;
} row_node;

extern label_node *lbl_head; /* Label table head */
extern label_node *dc_head; /* Data segment head */


label_node *create_label(int line_init, char *label_name,Label_Type label_type); /* This function creates a word_node according to the label name, type, initiated address and sets everything else to NULL */

void add_label (int line_init, char *label_name,Label_Type label_type); /* This function adds a label to the list */

label_node *label_exists(char *curr_label); /* This function checks if a label exists in the data list  */

void printList(int num); /*prints: 1 for label list, 2 for dc list, 3 for cmd list */

void free_list(int num); /* Frees the list, 1 for label list, 2 for dc list, 3 for cmd list*/

row_node *create_row(int address); /* This function creates a row_node */

void add_row(label_node *cur_label, int address); /* This function adds an address to the specific label node  */

/**********************
**** ERROR HANDLER ****
**********************/
void error_manager(ErrorCode errorCode); /* Prints the appropriate error message */

/***************************
****** MAIN FUNCTIONS *****
***************************/
void preAssembler(FILE *fp, char* clean_file_name); /* Inputs all macros */ 

void scan_file(FILE *file); /* Scans the .am file for the first time */

int check_first_word (char *word); /* Checks the first word in every line */


#endif
