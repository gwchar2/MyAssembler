#include "../include/assembler.h"

int error_manager(ErrorCode errorCode) {
    err_flag++;
    switch (errorCode-1) {
        case ERR_UNDEFINED_REGISTER:
            fprintf(stderr, "Error: Undefined register name in line %d\n", curr_line_number);
            break;
        case ERR_UNDEFINED_COMMAND:
            fprintf(stderr, "Error: Undefined command name in line %d\n", curr_line_number);
            break;
        case ERR_EXTRANEOUS_TEXT:
            fprintf(stderr, "Error: Extraneous text after end of command in line %d\n", curr_line_number);
            break;
        case ERR_EXTRANEOUS_LABEL:
            fprintf(stderr, "Error: Label length in line [%d] exceeds the allowable limit \n", curr_line_number);
            break;
        case ERR_UNDEFINED_ARGUMENT:
            fprintf(stderr, "Error: Undefined argument in line %d\n", curr_line_number);
            break;
        case ERR_MISSING_ARGUMENT:
            fprintf(stderr, "Error: Missing argument in line %d\n", curr_line_number);
            break;
        case ERR_ILLEGAL_COMMA:
            fprintf(stderr, "Error: Illegal comma in line %d\n", curr_line_number);
            break;
        case ERR_MISSING_COMMA:
            fprintf(stderr, "Error: Missing comma in line %d\n", curr_line_number);
            break;
        case ERR_MULTIPLE_CONSECUTIVE_COMMAS:
            fprintf(stderr, "Error: Multiple consecutive commas in line %d\n", curr_line_number);
            break;
        case ERR_SEGMENTATION_FAULT:
            fprintf(stderr, "Error: Segmentation fault in line %d\n", curr_line_number);
            break;
        case ERR_REDEFINITION_MACRO:
            fprintf(stderr, "Error: Redefinition of macro name in line %d\n", curr_line_number);
            break;
        case ERR_SIZE_LEAK:
            fprintf(stderr, "Error: Line [%d] is over 80 characters long \n", curr_line_number);
            break;
        case ERR_DUPLICATE_LABEL:
            fprintf(stderr, "Error: Label in line [%d] is a duplicate! \n", curr_line_number);
            break;




        default:
            fprintf(stderr, "Error: Unknown error in line %d\n", curr_line_number);
            break;
    }
}
