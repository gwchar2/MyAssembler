#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assembler.h"


void addFileEnding(char *file_name) {
    char *ending = ".as" ;
    strcat(file_name,ending);
}