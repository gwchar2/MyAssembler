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
#define NUM_OF_REGS 10
#define MIN_ARGV 2
#define FILE_NAME_LEN 50
#define CMD_NAME_LEN 3
#define MAX_LINE_LEN 81
#define MAX_ENDING_LEN 11
#define BIN_WORD_LEN 14
#define FIRST_GROUP_VARS 2
#define SECOND_GROUP_VARS 1
#define THIRD_GROUP_VARS 0
#define COMMA ','
#define SPACE_COMMA_DEL ", \f\r\t\v\n"
#define BITS_IN_INT 12
#define MAX_12BITS 2047
#define MIN_12BITS -2048
#define RS_SHIFT 3
#define RT_SHIFT 0
#define RT_BIT_FIELD 9
#define LSB 13
#define RSA_FIELD 8
#define RTA_FIELD 10
#define OPCODE_BIN_LEN 4
#define OPCODE_FIELD 4
/* Checks if the memory for (C) was allocated properly */
#define check_allocation(c)\
        if (c == NULL){\
            fprintf(stderr, "Error allocating memory %s",strerror(errno));\
            exit(EXIT_FAILURE);\
        }
#define WHITESPACE(c) ((c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'))     /* Checks if a character is a whitespace */
#define error(c) \
    do { \
        errorCode = c; \
        error_manager(errorCode); \
    } while(0)


/********************************* 
****** GLOBAL INTS & ENUMS *******
**********************************/
typedef enum ErrorCode{
    ERR_UNDEFINED_REGISTER,
    ERR_UNDEFINED_COMMAND,
    ERR_EXTRANEOUS_TEXT,
    ERR_INVALID_LABEL,
    ERR_UNDEFINED_ARGUMENT,
    ERR_MISSING_ARGUMENT,
    ERR_ILLEGAL_COMMA,
    ERR_MISSING_COMMA,
    ERR_MISSING_PARENTHESES,
    ERR_MULTIPLE_CONSECUTIVE_COMMAS,
    ERR_SEGMENTATION_FAULT,
    ERR_REDEFINITION_MACRO,
    ERR_ILLEGAL_ADDRESSING,
    ERR_IMM_OVERFLOW,
    ERR_SIZE_LEAK,
    ERR_DUPLICATE_LABEL,
} ErrorCode; 

typedef enum Label_Type{
    CMD_LABEL,
    DEF_LABEL, 
    ENTRY_LABEL,
    EXTERN_LABEL,
    STRING_LABEL,
    DATA_LABEL,
    INVALID
} Label_Type;

enum ending_type {
        as, 
        am,
        o,
        external, 
        entries
} ;

extern char *commands[NUM_OF_CMDS];                                                     /* Global Commands array */
extern char *registers[NUM_OF_REGS];                                                    /* Global Register array */
extern int curr_line_number;                                                            /* Current file number counter */
extern int IC;                                                                          /* IC Counter */
extern int DC;                                                                          /* DC Counter */
extern int err_flag;                                                                    /* Global error flag */
extern ErrorCode errorCode;                                                             /* Global error variable */

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
    char *cmd_binary;
    int sourceAdd;
    int targetAdd;
    int cmd_num;
    char *source1_binary;
    char *source2_binary;
    char *target1_binary;
    char *target2_binary; 
    struct Cmd_node *next_cmd; /* Next cmd */
    struct Label_node *next_label; /* Next label (null until merging with DC ) */
} cmd_node;

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

    struct Data_node *data_node;                                                        /* The data list */
    struct Row_node *row_list;                                                          /* The row list (the rows which it appears in )*/
    struct Label_node *next_label;                                                      /* Next label */
    struct Label_node *prev_label;                                                      /* Prev label */
    struct Label_node *next_entry;
    struct Label_node *next_extern;
    struct Label_node *next_dc;                                                   
} label_node;

typedef struct Data_node{                                                               /* For string, we put in ascii values */
    int data;
    struct Data_node *next_data;
}data_node;

typedef struct Row_node{
    int address;
    struct Row_node *next_row;
} row_node;

                                                           
extern label_node *lbl_head;                                                            /* Label table head */
extern label_node *entry_head;                                                          /* Entry list head */
extern label_node *extern_head;                                                         /* Extern list head */
extern label_node *dc_head;                                                             /* Data segment list head */
extern cmd_node *cmd_head;                                                              /* Instruction segment head */
extern cmd_node *new_cmd; 
extern char *rest_of_line;                                                              /* this pointer will always pont to the rest of the input line that wans't proccessed yet. */


label_node *create_label(int line_init,int definedData,char *label_name,int entry_count,Label_Type label_type); /* This function creates a word_node according to the label name, type, initiated address and sets everything else to NULL */

row_node *create_row(int address); /* This function creates a row_node */

data_node *create_data(int data, label_node *label_node); /* This function creates a data_node */

label_node *label_exists(char *curr_label); /* This function checks if a label exists in the data list  */

void *add_label(int line_init,int definedData,char *label_name,int entry_count,Label_Type label_type); /* This function adds a label to the list */

void *add_row(label_node *cur_label, int address); /* This function adds an address to the specific label node  */

void *add_data(int data,label_node *label_node); /* This function adds a data node to the data list in a label */


void *add_entry(label_node *label_node);  /* This function adds a an entry node to the entry list */

void *add_extern(label_node *label_node); /* This function adds a an extern node to the extern list */

void *add_cmd(cmd_node *some_node); /* This function adds a cmd node to the cmd list */

/*void *add_entry(label_node *label_node); */ /* This function adds a an entry node to the entry list */

void *add_extern(label_node *label_node); /* This function adds a an extern node to the extern list */

void *add_dc(label_node *label_node); /* This function adds a data node to the data segment list */

void printList(int num); /* Prints: 1 for label list, 2 for dc list, 3 for cmd list */

void print_label_guide(); /* Prints the guide for the table */

void free_list(int num); /* Frees the list, 1 for label list, 2 for dc list, 3 for cmd list*/


/*************************************
********** QUALITY OF LIFE ***********
*************************************/
int checkWordInArray(char **words, char* targetWord); /* This function checks to see if a targetWord exists in a word_array Returns 1 if it is, 0 if it isnt.*/

char *addFileEnding(char *file_name, int type); /* Adds the appropriate file ending */

void binToFour(FILE *obj_fp, char *str); /* Translates a string of 14 binary characters, to encrypted base 4 */

int strToInt(char *string); /* Calculates the given string to an int value. */

FILE *openFile(char *clean_file_name, int num); /* Opens a file with a specific ending. */
/*************************************
*********** ERROR HANDLER ************
*************************************/
void error_manager(ErrorCode errorCode); /* Prints the appropriate error message */



/*************************************
*********** LABEL HANDLER ************
*************************************/
int check_first_word (char *word); /* This function receives a word and checks what type of label it can potentialy be. */

Label_Type getLabelType(char *pointer); /* This function receives a word and returns the type of label it represents */

int check_label(char *p_copy,Label_Type label_type); /* Checks if a label is according to the demands, returns 1 if yes 0 if no */

int check_alpha(char *pointer); /* Checks if a string is all alphabetical letters returns 1 of yes 0 if no */

/************************************
***** COMMAND PARSER FUNCTIONS ******
*************************************/
void check_command(char *string) ;
int valid_command_name(char *cmd);
void getNumOfVars();
int sourceOpCheck(char **token);
int isIndex(char *input, int index, label_node *baseLabel) ;
int targetOpCheck(char *token);
int immProcessor(char *token, int *immNum);
int labelPrecessor(label_node *labelOp, int *labelVal) ;
int isNumber(char *imm, int *num);
char *BinTranslation12Bit(int num, int ARE) ;
int isReg(char *token) ;
char *RSBinTranslation(int reg_num) ;
char *RTBinTranslation(int reg_num);
char *cmdBinTranslation(int cmd_num, int sourceAdd, int targetAdd) ;
int rangeCheck(int num);
char *opcodeBinTranslation(int num) ;
char *combineRegBin(char *str1, char *str2);
int commaCheck(char *input_copy);
int checkExtra(char *extra) ;
cmd_node *create_cmd_node(int cmd_num) ;


/*************************************
*********** DEFINE HANDLER ***********
*************************************/

void define_handler(char *pointer, Label_Type label_type); /* This function handles all the different cases of .define */

void def_case_1(char *pointer, Label_Type label_type); /* Handles the first case of define data input. case 1:  .define label=num */

void def_case_2(char *pointer, Label_Type label_type,char *pointer2);/* Handles the first case of define data input. case 2:  .define label =num */

void def_case_3(char *pointer, Label_Type label_type,char *pointer2);/* Handles the first case of define data input. case 3:  .define label= num */

void def_case_4(char *pointer, Label_Type label_type ,char *pointer2);/* Handles the first case of define data input. case 4:  .define label = num */

/*************************************
********** EXTERN HANDLER ************
*************************************/

void extern_handler(char *pointer,Label_Type label_type); /* Handles extern labels */

/************************************
********** DSTRING HANDLER **********
*************************************/

int dstring_handler(char *pointer); /* This handels the .data and .string labels */

int check_data(char *p_copy,Label_Type label_type); /* This checks the data received in a .data / .string label */


void fetch_data(char *p_copy, label_node *temp_node); /* This fetches the date received from a .data / .string label */

/*************************************
********** MAIN FUNCTIONS ************
*************************************/
void preAssembler(FILE *fp, char* clean_file_name); /* Inputs all macros */ 

void scan_file(FILE *file); /* Scans the .am file for the first time */




















#endif
