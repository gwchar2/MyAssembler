#include <stdio.h>
#include <string.h>

char *combineRegBin(char *str1, char *str2) {
    int i;
    static char result[15] ;
    for (i-0; i<9; i++) {
        result[i] = *(str1+i) ;
    }
    for (i=9; i<=11; i++) {
        result[i] = *(str2+i) ;
    }
    result[12] = '0' ;
    result[13] = '0' ;
    result[14] = '\0';
    return result ;

}

int main () {
    char str1[15] ;
    strcpy(str1,combineRegBin("00000011100000", "00000000011100")) ;
    printf("%s\n", str1) ;

    return 0; 
}