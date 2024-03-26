#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"


int main(int argc,char *argv[]) {

    FILE *fp ;
    char file_name[FILE_NAME_LEN] ;
    int file_count = 0 ; /* pointer to argv index */

     /* checking arguments */
    if (argc < MIN_ARGV) /* no arguments */ {
        fprintf(stderr, "error: no arguments.\n");
        exit(1) ;
    }
    file_count++ ; /* argumants found. increase file count */
    /* opening file and checking success */
    strcpy(file_name, argv[1]) ;
    addFileEnding(file_name) ;
    fp = fopen(file_name, "r") ;
    if (fp==NULL){
        fprintf(stderr, "error: file opening failed.\n");
        exit(1) ;
    }

    else 
    printf("success\n");

    pre_assembler(fp) ;

    
    
    return 0;
}
