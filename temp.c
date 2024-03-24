
char command_line[MAX_LINE_LEN] ; /* command line recieved from user */
    char command_line_cpy[MAX_LINE_LEN] ; /* saving a copy */
    int flag = 0, error_num = 0 ;
    data cur_data ; /* current command data */

/* getting input from user */
    while(flag == 0) {
        if (fgets(command_line, MAX_LINE_LEN, stdin)==NULL){ /* reached EOF without "stop" command */
            printf("Error: reaching end-of-file without stop command\n") ;
            flag = 1 ;
            exit(1) ;
        }
        
        printf("%s\n", command_line) ;
        error_num = 0 ;   
        strcpy(command_line_cpy,command_line) ;
        strcpy(cur_data.input,command_line) ;
        strcpy(cur_data.input_copy,command_line) ;

        error_num = validateInput(&cur_data, *mats, commands[0]) ; /* call validation function */

        if (error_num!=0)
           printf("Error: %s\n", (errors[error_num-1].error_m)); /* print the error message */
        else
            exFunc(&cur_data, *mats) ; /* call the executive function */  
        printf("Next command: ") ;
        }
