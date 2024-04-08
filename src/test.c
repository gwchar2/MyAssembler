#include <stdio.h>
#include <string.h>



int main () {
    char str[] = "   hello world" ;
    char *tok1 = NULL ;
    char *tok2 = NULL ;
    tok1 = strtok(str," ");
    tok2 = strtok(str," ") ; 
    printf("str: %s\n", str);
    printf("stok1: |%s|\n", tok1);
    printf("tok2: %s\n", tok2);

    return 0; 
}