#define NUM_OF_CMDS 16
#define MIN_ARGV 2
#define FILE_NAME_LEN 50
#define MAX_LINE_LEN 80
#define MAX_ENDING_LEN 11
#define BIN_WORD_LEN 14




char *addFileEnding(char *file_name, int type) ;
enum ending_type {as, am, o, external, entreis} ;

/* The Error struct conatins the error messages and an error number */
struct Error {
    int error_num ;
    char *error_m ;
    } ;

    typedef struct mac_text {
        char *text;
        struct mac_text *next;
    } mac_text ;

    typedef struct macro {
        char *mac_name ;
        struct mac_text *text ;
        struct macro *next;
    } macro ;

    extern char *commands[];
    extern struct Error errors[];
    extern macro *head ; 

    /* pre-assembler functions prototypes */

    void preAssembler(FILE *fp, char* clean_file_name) ;
    mac_text *createText (char *line) ;
    macro *createMacro (char *name, char *line) ;
    void addMcr(char *name, FILE *fp, macro **head) ;
    void addText(macro *cur_mac, char *line) ;
    macro *searchMcrList(char *word, macro *head) ;
    void copyMcrText(macro *cur_mac, FILE *newP) ;
    int validateMcrName(char *mcr_name, macro *head) ;
    void errorHandler() ;

void binToFour(char *clean_file_name, char *str) ;