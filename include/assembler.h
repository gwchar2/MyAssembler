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
#define MAX_LINE_LEN 81
#define MAX_ENDING_LEN 11
#define BIN_WORD_LEN 14
#define NUM_OF_REGS 9
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
    ERR_EXTRANEOUS_LABEL,
    ERR_UNDEFINED_ARGUMENT,
    ERR_MISSING_ARGUMENT,
    ERR_ILLEGAL_COMMA,
    ERR_MISSING_COMMA,
    ERR_MULTIPLE_CONSECUTIVE_COMMAS,
    ERR_SEGMENTATION_FAULT,
    ERR_REDEFINITION_MACRO,
    ERR_SIZE_LEAK,
    ERR_DUPLICATE_LABEL
} ErrorCode; 

typedef enum Label_Type{
    CMD_LABEL,
    DATA_LABEL, 
    ENTRY_LABEL,
    DEF_LABEL,
    EXTERNAL_LABEL,
    STRING_LABEL,
    INVALID
} Label_Type;

enum ending_type {
        as, 
        am,
        o,
        external, 
        entreis
};

extern char *commands[NUM_OF_CMDS];
extern char *registers[NUM_OF_REGS];
extern int curr_line_number;
extern int IC;
extern int DC;
extern int err_flag;
extern ErrorCode errorCode;

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
    int address;                                                            /* The instruction count */
    int total_vars;                                                         /* The total amount of variables it holds */
    int L;
    char *binary_cmd;
    char *var1_binary;
    char *var2_binary;
    char *var3_binary;
    char *var4_binary; 
    struct Cmd_node *next_cmd; /* Next cmd */
    struct Label_node *next_label; /* Next label (null until merging with DC ) */
} cmd_node;

extern cmd_node *cmd_head; /* Instruction segment head */

/************************************
********** LABEL STRUCTURE **********
*************************************/
typedef struct Label_node{
    int line_init;                                                                      /* Address the label was intialized in */
    int data_count;                                                                     /* Total amount of data stored in the label */
    int definedData;                                                                    /* The immediate value of a define label or extern (-1)*/
    char *label_name;                                                                   /* Label name */
    int entry_count;                                                                    /* 0 if an entry doesnt have a 'partner' yet, 1 if it does. */
    enum Label_Type label_type;                                                         /* Type of label */

    struct data *data;                                                                  /* The data list */
    struct Row_node *row_list; /* The row list (the rows which it appears in )*/
    struct Label_node *next_label; /* Next label */
    struct Label_node *prev_label; /* Prev label */
} label_node;

typedef struct data{ /* For string, we put in ascii values */
    int data;
    struct data *next_data;
} data;

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
/************************************
********** QUALITY OF LIFE **********
*************************************/
int checkWordInArray(char **words, char* targetWord); /* This function checks to see if a targetWord exists in a word_array */

char *addFileEnding(char *file_name, int type); /* Adds the appropriate file ending */

void binToFour(FILE *obj_fp, char *str); /* Translates a string of 14 binary characters, to encrypted base 4 */
/************************************
*********** ERROR HANDLER ***********
*************************************/
void error_manager(ErrorCode errorCode); /* Prints the appropriate error message */



/************************************
*********** LABEL HANDLER ***********
*************************************/
int check_first_word (char *word); /* This function receives a word and checks what type of label it can potentialy be. */

Label_Type getLabelType(char *pointer); /* This function receives a word and returns the type of label it represents */














/************************************
********** MAIN FUNCTIONS ***********
*************************************/
void preAssembler(FILE *fp, char* clean_file_name); /* Inputs all macros */ 

void scan_file(FILE *file); /* Scans the .am file for the first time */




















#endif
