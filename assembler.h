#define NUM_OF_CMDS 16
#define MIN_ARGV 2
#define FILE_NAME_LEN 50
#define MAX_LINE_LEN 80


void addFileEnding(char *file_name);
/* The Error struct conatins the error messages and an error number */
struct Error {
    int error_num ;
    char *error_m ;
    };


typedef struct mac_text {
    char *text;
    struct text *next;
}

typedef struct macro {
    char *mac_name ;
    struct mac_text *text ;
    struct macro *next;
}


