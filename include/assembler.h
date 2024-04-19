#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

/******************************* 
****** DEFINES AND MACROS ******
********************************/
#define PREFIX 10
#define NUM_OF_CMDS 16                                                                              /* The number of valid commands in the program */
#define NUM_OF_REGS 10                                                                              /* The number of valid registers in the program */
#define MAX_REGS 4                                                                                  /* The maximum amount of arguments used in a command */
#define MIN_ARGV 2                                                                                  /* The minimum amount of variables received in the CLI */
#define FILE_NAME_LEN 290                                                                           /* Max amount of characters in a file name in a 32 bit system according to ASCII Standard*/
#define MAX_LINE_LEN 81                                                                             /* The maximum length of a received line in the program */
#define MAX_ENDING_LEN 11                                                                           /* The maximum length of a file ending */
#define BIN_WORD_LEN 14                                                                             /* The length of 14 binary array */
#define BITS_IN_INT 12                                                                              /* The amount of bits in an integer */
#define MAX_12BITS 2047                                                                             /* The highest number achievable with 12 bits */
#define MIN_12BITS -2048                                                                            /* The lowest number achievable with 12 bits */
#define RS_SHIFT 3                                                                                  /* The amount to shift RS to its spot */
#define RT_SHIFT 0                                                                                  /* The amount to shift RT to its spot */
#define RT_BIT_FIELD 9                      
#define LSB 13                                                                                      /* The location of the LSB */
#define RSA_FIELD 8                         
#define RTA_FIELD 10                        
#define OPCODE_BIN_LEN 4                                                                            /* The length of a binary op code */
#define OPCODE_FIELD 4                                                                              /* Size of OPCODE field */
#define RAM 4096                                                                                    /* The total amount of RAM in the program */
#define MAX_LABEL_NAME 32                                                                           /* The max length of a label in the program */
/* Checks if the memory for (C) was allocated properly */
#define check_allocation(c)\
        if (c == NULL){\
            errorCode = ERR_SIZE_LEAK; \
            error_manager(errorCode); \
            exit(EXIT_FAILURE);\
        }
#define WHITESPACE(c) ((c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'))     /* Checks if a character is a whitespace */
#define error(c) \
    do { \
        errorCode = c; \
        error_manager(errorCode); \
    } while(0)


/********************************** 
********** GLOBAL ENUMS ***********
***********************************/
/* Different types of errors */
typedef enum ErrorCode{
    ERR_UNDEFINED_REGISTER,
    ERR_UNDEFINED_REGISTER_TYPE,
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
    ERR_ILLEGAL_BRACKETS,
    ERR_IMM_OVERFLOW,
    ERR_SIZE_LEAK,
    ERR_DUPLICATE_LABEL,
    ERR_FILE_NAME,
    ERR_FILE_ARGS,
    ERR_ERR_FLAG,
    ERR_UNDEFINED_LBL_USE,
    ERR_LBL_PARTNER,
    ERR_FOLDER
} ErrorCode; 

/* A list of label types*/
typedef enum Label_Type{
    CMD_LABEL,
    DEF_LABEL, 
    ENTRY_LABEL,
    EXTERN_LABEL,
    STRING_LABEL,
    DATA_LABEL,
    INVALID
} Label_Type;

/* File ending types */
enum ending_type {
        as, 
        am,
        o,
        external, 
        entries
};

/* A list of different register types */
typedef enum Reg_Type{
    IMM, /* 0 */
    IMM_FR_LBL, /* 1 */
    LABEL, /* 2 */
    IMM_LBL, /* 3 */
    REG,  
    EXTERN,
    IMPLICIT
} Reg_Type;

/* A list of valid instructions */
typedef enum Instruction {
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    HLT,
    UNKNOWN
} Instruction;

/************************************
************ STRUCTURES *************
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

typedef struct Pointer_node{                                                            /* This structure will help us free all the dynamicaly allocated data */
    char *pointer;
    struct Pointer_node *next_pointer;
} pointer_node;

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
    struct Label_node *next_entry;                                                      /* The next entry label node */
    struct Label_node *next_extern;                                                     /* The next extern label node */
    struct Label_node *next_dc;                                                         /* The next node in the data segment */    
    struct Label_node *next_cmd_label;                                                  /* The next node representing a command label */                                               
} label_node;

typedef struct Data_node{                                                               
    int data;                                                                           /* The piece of data stored in the DATA or STRING labels */
    struct Data_node *next_data;                                                        /* A pointer to the next piece of data stored */
}data_node;

typedef struct Row_node{
    int address;                                                                        /* Holds the address where an external label appears */
    struct Row_node *next_row;                                                          /* Points to the next row node (holds another address) */
} row_node;

typedef struct Cmd_node{
    enum Instruction instruction;                                                       /* This enum represents the type of command */
    char *bin_value;                                                                    /* This holds the binary value of the command */
    int instruction_type;                                                               /* If has 2 registers, than 2, else 1 */
    int address;                                                                        /* The IC address of the cmd */
    int total_len;                                                                      /* Total amount of variables */
    int addressStyleRS;                                                                 /* addressing style of the RS in base 10*/
    int addressStyleRT;                                                                 /* addressing style of the RT in base 10*/
    struct Cmd_node *next_cmd;                                                          /* A pointer to the next command in the instruction segment */
    struct Reg_node *next_reg;                                                          /* A pointer to the register list */
} cmd_node;

typedef struct Reg_node{
    char *bin_value;                                                                    /* Holds the binary value of the register */
    int data;                                                                           /* Holds the base 10 data */
    int ARE;                                                                            /* Holds the correct ARE*/
    int RS;                                                                             /* RS / RT flag. 1 if they are what they are.*/
    int RT;                                                                             /* RT Flag */
    int IC;                                                                             /* The IC address of the register data. */
    int row;                                                                            /* The line in the file where the register was first initiated. */
    enum Reg_Type reg_type;                                                             /* This enum represents the reg type */
    char *label_name;                                                                   /* If the register is a label, this holds the value */
    struct Reg_node *next_reg;                                                          /* A pointer to the next register */
} reg_node;
/************************************
******** EXTERNAL VARIABLES *********
*************************************/
                                                            
extern char *curr_file;                                                                 /* This pointer will always point to the current file that is open. */
extern char *commands[NUM_OF_CMDS];                                                     /* Global Commands array */
extern char *registers[NUM_OF_REGS];                                                    /* Global Register array */
extern int curr_line_number;                                                            /* Current file number counter */
extern int IC;                                                                          /* IC Counter */
extern int DC;                                                                          /* DC Counter */
extern int err_flag;                                                                    /* Global error flag */
extern ErrorCode errorCode;                                                             /* Global error variable */
extern label_node *lbl_head;                                                            /* Label table head */
extern label_node *entry_head;                                                          /* Entry list head */
extern label_node *extern_head;                                                         /* Extern list head */
extern label_node *dc_head;                                                             /* Data segment list head */
extern label_node *cmd_label_head;                                                      /* CMD Labels list head */
extern macro *head;                                                                     /* Macro list head */
extern cmd_node *cmd_head;                                                              /* Instruction segment list head */
extern pointer_node *ptr_head;                                                          /* Pointer list head */

/************************************
************ LABEL NODES ************
*************************************/
                                                                                        /* This function creates a word_node according to the label name, type, initiated address and sets everything else to NULL */
label_node *create_label(int line_init,int definedData,char *label_name,int entry_count,Label_Type label_type); 

label_node *label_exists(char *curr_label);                                             /* This function checks if a label exists in the data list  */

void *add_label(int line_init,int definedData,char *label_name,int entry_count,Label_Type label_type); /* This function adds a label to the list */

void *add_cmd_label(label_node *some_node);                                             /* This function adds a cmd label node to the cmd label list */

void *add_entry(label_node *label_node);                                                /* This function adds a an entry node to the entry list */

void *add_extern(label_node *label_node);                                               /* This function adds a an extern node to the extern list */

void *add_entry(label_node *label_node);                                                /* This function adds a an entry node to the entry list */

void *add_extern(label_node *label_node);                                               /* This function adds a an extern node to the extern list */

void *add_dc(label_node *label_node);                                                   /* This function adds a data node to the data segment list */

void printList(int num);                                                                /* Prints: 1 for label list, 2 for dc list, 3 for cmd list */

void print_label_guide();                                                               /* Prints the guide for the table */

void freeLists();                                                                       /* Frees all the lists */

/************************************
************* DATA NODES ************
*************************************/

data_node *create_data(int data, label_node *label_node);                               /* This function creates a data_node */

void *add_data(int data,label_node *label_node);                                        /* This function adds a data node to the data list in a label */

/************************************
************ COMMAND NODES **********
*************************************/

cmd_node *create_cmd(Instruction instruction);                                          /* This function creates a command node */

void *add_cmd(Instruction instruction);                                                 /* This function adds a command node to the command list */

reg_node *create_reg(Reg_Type reg_type, int value);                                     /* This function creates a register node */

reg_node *add_reg (cmd_node *some_cmd, Reg_Type reg_type, int value);                   /* This function adds a register node to the list */

void check_cmd(char *full_line);                                                        /* This function checks if the received line is a valid command input */


/************************************
************ MACRO NODES ************
*************************************/

mac_text *createText (char *line);                                                      /* creating a new mac_text struct and setting line in it's text member */

macro *createMacro (char *name, char *line);                                            /* This function recieves a word and a number and creates a new Word struct with that word number and sets the next pointer to null */

macro *searchMcrList(char *word, macro *head);                                          /* this functions get's a string and the head of the list, and searches for it in the macros list. if founded, it returns a pointer to it's text. else, null. */

void copyMcrText(macro *cur_mac, FILE *newP);                                           /* this functions copies a given macro content to the new file */

int validateMcrName(char *mcr_name, macro *head);                                       /* this functiond checks if the macro name is valid. returns 1 for valid. 0 for invalid. */

void addMcr(char *name, FILE *fp, macro **head);                                        /* this functions adds a new macro defenition to the macro list. it returns when reaching endmcr or eof */

void addText(macro *cur_mac, char *line);                                                           

/*************************************
************* ROW NODES **************
*************************************/
row_node *create_row(int address,label_node *label_node);                               /* This function creates a row_node */

void *add_row(label_node *cur_label, int address);                                      /* This function adds an address to the specific label node  */


/*************************************
*********** POINTER NODES ************
*************************************/
pointer_node *create_pointer(char *ptr);                                                /* This function creates a pointer node */

void *add_ptr(char *pointer);                                                           /* This function adds a pointer node to the pointer list */

/*************************************
*********** MAIN FUNCTIONS ***********
*************************************/

void preAssembler(FILE *fp, char* clean_file_name);                                     /* Inputs all macros */ 

void scan_file(char *clean_file_name);                                                  /* Scans the .am file for the first time */

void makefiles(char *clean_file_name);                                                  /* Makes the required files */

/*************************************
********** DEFINE HANDLER ************
*************************************/

void define_handler(char *pointer, Label_Type label_type);                              /* This function handles all the different cases of .define */

void def_case_1(char *pointer, Label_Type label_type);                                  /* Handles the first case of define data input. case 1:  .define label=num */

void def_case_2(char *pointer, Label_Type label_type,char *pointer2);                   /* Handles the first case of define data input. case 2:  .define label =num */

void def_case_3(char *pointer, Label_Type label_type,char *pointer2);                   /* Handles the first case of define data input. case 3:  .define label= num */

void def_case_4(char *pointer, Label_Type label_type ,char *pointer2);                  /* Handles the first case of define data input. case 4:  .define label = num */


/************************************
********** DSTRING HANDLER **********
*************************************/

int dstring_handler(char *pointer);                                                     /* This handels the .data and .string labels */

int check_data(char *p_copy,Label_Type label_type);                                     /* This checks the data received in a .data / .string label */

void fetch_data(char *p_copy, label_node *temp_node);                                   /* This fetches the date received from a .data / .string label */

/*************************************
*********** ERROR HANDLER ************
*************************************/
void error_manager(ErrorCode errorCode);                                                /* Prints the appropriate error message */

/*************************************
********** EXTERN HANDLER ************
*************************************/

void extern_handler(char *pointer,Label_Type label_type);                               /* Handles extern labels */
/************************************
************ CMD HANDLER ************
*************************************/

Instruction get_cmd(char *cmd);                                                         /* This function returns the enum equal to the received command */

int countCommas(int instruction_type,char *full_line);                                  /* This function counts the amount of commas in the received line */

int countSquares(cmd_node *cmd_node, char *full_line);                                  /* This function counts the amount of brackets in the received line */

int checkIfReg(char *reg);                                                              /* This function checks if the received string is a register */
    
int checkImmDef(char *reg);                                                             /* This function checks if the received string can be an immediate or a define*/

int checkLblImmDef(char *reg);                                                          /* This function checks if the received string can be an immediate or define for a label[something] */                       


/************************************
*********** BINARY HANDLER **********
*************************************/

char *cmdBinTranslation(int cmd_num, int sourceAdd, int targetAdd);                     /* This translates the command to the correct 14 bit binary */

char *opcodeBinTranslation(int num);                                                    /* This translates the op code to 14 bit binary */

char *BinTranslation12Bit(int num, int ARE);                                            /* This translates the received immediate to 14 bit binary */

char *RSBinTranslation(int reg_num);                                                    /* This translates the RS to 14 bit binary word*/

char *RTBinTranslation(int reg_num);                                                    /* This function translates the RT to a 14 bit binary word*/

char *combineRegBin(char *str1, char *str2);                                            /* This function combines 2 registers to 1 14 binary word */

char *BinTranslation14Bit(int num);                                                     /* This function translates a number to a string of 14 binary  */

void binToFour(FILE *obj_fp, char *str);                                                /* Translates a string of 14 binary characters, to encrypted base 4 */

void translateToBin();                                                                  /* This function translates the entire command list to binary */


/*************************************
*********** FILE HANDLER ************
*************************************/

char *addFileEnding(char *file_name, int type);                                         /* Adds the appropriate file ending */

FILE *openFile(char *clean_file_name, int num);                                         /* Opens a file with a specific ending. */

void *findNotEntry(char *name);                                                         /* This function finds the first label with the same name as received, that is not an entry. */

void make_extern();                                                                     /* This function makes the extern file */

void make_entry();                                                                      /* This function makes the entry file */

void make_object();                                                                     /* This function makes the object file */

void mergeSegments();                                                                   /* This function merges IC and DC lists, and updates all the required addresses in both segments. */

void fixEntrys();                                                                       /* This function fixes the addresses in entries, and also makes sure every entry has a 'partner' */

void fixCMDs();

/*************************************
*********** LABEL HANDLER ************
*************************************/
int check_first_word (char *word);                                                      /* This function receives a word and checks what type of label it can potentialy be. */

Label_Type getLabelType(char *pointer);                                                 /* This function receives a word and returns the type of label it represents */

int check_label(char *p_copy,Label_Type label_type);                                    /* Checks if a label is according to the demands, returns 1 if yes 0 if no */

int check_alpha(char *pointer);                                                         /* Checks if a string is all alphabetical letters returns 1 of yes 0 if no */

/*************************************
************* FUNCTIONS **************
*************************************/

int checkRegs(char *targetWord);                                                        /* This function checks to see if a targetWord exists in the register array. Returns 1 if it is, 0 if it isnt.*/

int checkCmds(char *targetWord);                                                        /* This function checks to see if a targetWord exists in the commands array. Returns 1 if it is, 0 if it isnt.*/

int strToInt(char *string);                                                             /* Calculates the given string to an int value. */

void resetGlobals();                                                                    /* This function resets the global variables */

void freeLists(); /* This function frees all the lists */

void freeDataList(data_node *head); /* Frees the sub data List */

void freeRowList(row_node *head); /* Frees the sub row list */

void freeRegs(reg_node *head); /* Frees the sub register List */

void freeMacro(mac_text *head); /* Frees the macro list */
#endif