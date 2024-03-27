#define NUM_OF_CMDS 16
#define MIN_ARGV 2
#define FILE_NAME_LEN 50
#define MAX_LINE_LEN 80
/* Checks if the memory for (C) was allocated properly */
#define check_allocation(c)\
        if (c == NULL){\
            fprintf(stderr, "Error allocating memory %s",strerror(errno));\
            exit(EXIT_FAILURE);\
        }  

void addFileEnding(char *file_name);
/* The Error struct conatins the error messages and an error number */
struct Error {
    int error_num ;
    char *error_m ;
    } ;

    typdef struct mac_text {
        char *text;
        struct text *next;
    }
    typedef struct macro {
        char *mac_name;
        struct mac_text *text ;
        struct macro *next;
    }

/**********************************************
************     Label Structure    ************
**********************************************/
typedef struct Label_node{
    int line_initialized; /* The specific row where the Label was first initialized - since can only be used after initialized! */
    char line[MAX_LINE_LEN]; /* The full row it is initialized with */
    char *label_name; /* Name of the label */
    enum Label_Type label_type; /* The type of label, as written bellow */
    union Data data; /* The data stored in the label, can hold 4 different types, according to the label_type - integer is for .define ONLY*/
    struct Row_node *row_list_head;  /* Head of the list of appearences */
    struct Label_node *next_label; /* The next label in the code */
} label_node;

union Data {
    char string[MAX_LINE_LEN]; /* label_type = STRING_LABEL */
    int array[max_line_len]; /* label_type = DATA_LABEL OR .define */
};

/*A list of appearences of a specific label */
typedef struct Row_node {
    int address;
    struct Row_node *next_row;
} row_node;

/* An enum of label types */
enum Label_Type{
    CMD_LABEL,
    STRING_LABEL, /* char array */
    DATA_LABEL, /*num array */
    ENTRY_LABEL,
    EXTERNAL_LABEL,
    DEFINE_LABEL /* .define ls = 2 */
};


/**********************************************/
extern char *commands[]; 