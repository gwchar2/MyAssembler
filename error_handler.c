#include "header.h"

void error_manager(ErrorCode errorCode) {
    err_flag++;
    switch (errorCode) {
        case ERR_UNDEFINED_REGISTER:
            fprintf(stderr, "Error: Undefined register name\n");
            break;
        case ERR_UNDEFINED_COMMAND:
            fprintf(stderr, "Error: Undefined command name\n");
            break;
        case ERR_EXTRANEOUS_TEXT:
            fprintf(stderr, "Error: Extraneous text after end of command\n");
            break;
        case ERR_UNDEFINED_ARGUMENT:
            fprintf(stderr, "Error: Undefined argument\n");
            break;
        case ERR_MISSING_ARGUMENT:
            fprintf(stderr, "Error: Missing argument\n");
            break;
        case ERR_ILLEGAL_COMMA:
            fprintf(stderr, "Error: Illegal comma\n");
            break;
        case ERR_MISSING_COMMA:
            fprintf(stderr, "Error: Missing comma\n");
            break;
        case ERR_MULTIPLE_CONSECUTIVE_COMMAS:
            fprintf(stderr, "Error: Multiple consecutive commas\n");
            break;
        case ERR_SEGMENTATION_FAULT:
            fprintf(stderr, "Error: Segmentation fault\n");
            break;
        case ERR_REDEFINITION_MACRO:
            fprintf(stderr, "Error: Redefinition of macro name\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown error\n");
            break;
    }
}
