#define NUM_OF_CMDS 16
#define MIN_ARGV 2
#define FILE_NAME_LEN 50
#define MAX_LINE_LEN 80
#define MAX_ENDING_LEN 11
#define BIN_WORD_LEN 14
#define NUM_OF_REGS 9
#define check_allocation(c)\
        if (c == NULL){\
            fprintf(stderr, "Error allocating memory %s",strerror(errno));\
            exit(EXIT_FAILURE);\
        } 
   
extern char *commands[];
extern struct Error errors[]; 
extern char *registers[];
extern macro *head;
extern label_node *lbl_head; /* Label table head */
extern label_node *dc_head; /* Data segment head */
extern Cmd_node *cmd_head; /* Instruction segment head */

/* The Error struct conatins the error messages and an error number */
struct Error {
    int error_num;
    char *error_m;
};

typedef struct mac_text {
    char *text;
    struct mac_text *next;
} mac_text;

typedef struct macro {
    char *mac_name;
    struct mac_text *text;
    struct macro *next;
} macro;


    /* pre-assembler functions prototypes */
char *addFileEnding(char *file_name, int type);
enum ending_type {as, am, o, external, entreis};
void preAssembler(FILE *fp, char* clean_file_name);
mac_text *createText (char *line);
macro *createMacro (char *name, char *line);
void addMcr(char *name, FILE *fp, macro **head);
void addText(macro *cur_mac, char *line);
macro *searchMcrList(char *word, macro *head);
void copyMcrText(macro *cur_mac, FILE *newP);
int validateMcrName(char *mcr_name, macro *head);
void errorHandler();
void binToFour(char *clean_file_name, char *str);

/************************************
********** QUALITY OF LIFE **********
*************************************/
int checkWordInArray(char *words, char* targetWord); /* searches if a targetWord exists in an array of words*/

/************************************
********** COMMAND STRUCTURE ********
*************************************/
typedef struct Cmd_node{
    int address; /* The instruction count */
    int total_vars; /* The total amount of variables it holds */
    char *binary_cmd;
    char *var1_binary;
    char *var2_binary;
    char *var3_binary;
    char *var4_binary;
    struct Cmd_node *next_cmd; /* Next cmd */
}

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
    Bind_data *next_data;
};

typedef struct Row_node{
    int address;
    struct Row_node *next_row;
} row_node;

enum Label_Type{
    CMD_LABEL,
    DATA_LABEL, /* Includes string, but if it is string, it must have \0 as last data array (14 zeros)*/
    ENTRY_LABEL,
    EXTERNAL_LABEL
}
label_node *create_label(); /* Still in construction - This function creates a label_node */
void add_label (); /* Still in construction - This function adds a label to the label tabel */
label_node *searchLabelList(char *label, label_node **head); /* This function checks if a label exists in the list */  

row_node *create_row(int my_row); /* This function creates a row_node */
void add_row(label_node *cur_label, int address); /* This function adds an address to the specific label node  */
