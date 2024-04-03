#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main() {
    char line1[] = "label[ 3]" ;
    char line2[] = "label[     3]" ;
    
    int num ; 
    char label[64];
    sscanf(line1, "%[a-zA-Z0-9][ %d]", label, &num ) ;
    printf("label is - %s\n   number is - %d\n", label, num) ;
    /*sscanf(line1, "%s[ %d]", label, &num ) ;
    printf("label is - %s   number is - %d", label, num) ;*/

    return 0;
}