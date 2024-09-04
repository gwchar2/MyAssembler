#include "../include/assembler.h"



/* 
* This function creates a new .am file with the macros spreaded.
* The function goes through the .as file, when reaching a macro definition, 
* it saves it’s name and *content. later, it creates a new .am file and copies the original file,
* while replacing every macro use with it’s content.
* @param – fp – pointer to the original  .as file
* @param – clean_file_name – a string as recived from command line, representing the file’s name
 */
void preAssembler(FILE *fp, char* clean_file_name) {
    char *file_name = NULL;
    char *temp_name = NULL; 
    char cur_line[MAX_LINE_LEN];
    char copy_line[MAX_LINE_LEN];
    char *token = NULL;
    FILE *newP;
    macro *temp_mac = NULL;


    /* create a new .am file with the matching file name */
    /* adding the .am ending */
    temp_name = addFileEnding(clean_file_name,1);
    file_name = malloc(strlen(temp_name)+1);/* allocate memory for file name with ending */
    check_allocation(file_name);
    add_ptr(temp_name);
    add_ptr(file_name);
    strcpy(file_name,temp_name); /* file name updated with the correct .am ending */
    newP = fopen(file_name,"w");

    /* going through the source file, spreading macros to the new .am file */
    while((fgets(cur_line, MAX_LINE_LEN, fp)) != NULL) {
        strcpy(copy_line,cur_line); /* copying current line */
        token = strtok(copy_line," \n\t"); /* cut first word */
        temp_mac = searchMcrList(token, head);
        if (temp_mac != NULL) {/* word found in macro list */
            copyMcrText(temp_mac,newP); /* writing macro content in the new file */
            continue; /* get next line */
        }
        if (token != NULL){
            if (strcmp(token,"mcr") == 0){
                token = strtok(NULL," \n\t"); /* cut macro name */
                if (validateMcrName(token, head)) {/* return 1 if name is new and legal. else, returns 0;  */
                    addMcr(token,fp,&head); /* add macro to list with it's content. return after endmcr */
                    continue;
                }
                else {
                    error(ERR_REDEFINITION_MACRO);
                }
                continue;
            }
        }
        if (token != NULL){
            if (strcmp(token,"mcr") == 0){
                token = strtok(NULL," \n\t"); /* cut macro name */
                if (validateMcrName(token, head)) {/* return 1 if name is new and legal. else, returns 0;  */
                    addMcr(token,fp,&head); /* add macro to list with it's content. return after endmcr */
                    continue;
                }
                else {
                    error(ERR_REDEFINITION_MACRO);
                }
                continue;
            }
        }
        /* no macro defenition or use. copy line as is */
        
        fputs(cur_line,newP);
    }
    fclose(newP);
}

/* 
* This function scans the spreaded .am file and sends every line to it’s relevant process.
* @param – clean_file_name – the file name without it’s ending. 
* @return – after scanning and processing the whole file
*/
void scan_file(char *clean_file_name){
    char line[MAX_LINE_LEN];
    char *inputCopy = NULL;
    char *pointer = NULL;
    char *full_line = NULL;
    int flag;    
    FILE *fp;
    Label_Type label_type;

    /* Open the .am file */
    fp = openFile(clean_file_name,1);
    if (fp == NULL){
        error(ERR_FILE_NAME);
        return;
    }

    /* Go through the file as long as there is enough RAM */
    while (((IC + DC)-2) <= RAM) {
        
        /* Grab the next line from the file */
        if (fgets(line, MAX_LINE_LEN, fp) == NULL) {
            if (feof(fp)) {
                fclose(fp);
                break; 
            }
        }
        inputCopy = malloc(strlen(line) + 1); 
        check_allocation(inputCopy);
        strcpy(inputCopy, line); 
        
        /* Check to see if the file is shorter than 80 characters if it isnt, go next line, else, handle. */
        if (strlen(line) == MAX_LINE_LEN - 1 && line[MAX_LINE_LEN - 2] != '\n') {                                       /* Checking to see if the array is full without \n */
            error(ERR_SIZE_LEAK);
            /* Keeps grabbing the remainder of the line so that we skip to the next one!  */
            while (strlen(line) == MAX_LINE_LEN - 1 && line[MAX_LINE_LEN - 2] != '\n'){
                /* If we reach EOF or NULL just break. */
                if (fgets(line, MAX_LINE_LEN, fp) == NULL) {
                    if (feof(fp)) {
                        free(inputCopy);
                        fclose(fp);
                        break; 
                    }
                }
            } 
            curr_line_number++;
        }
        else{
            inputCopy = malloc(strlen(line) + 1); 
            check_allocation(inputCopy);
            strcpy(inputCopy, line); 
            /* Grab the first word in the line */
            pointer = strtok(inputCopy, " \t\n\r\f\v");                                                               
            switch (check_first_word(pointer)){
                /* Cases to ignore (; \n entry (at the moment))*/
                case 0:                                                                             
                    break;   
                /* define or externals */                                                                                           
                case 1:   
                    label_type = getLabelType(pointer);  
                    if (label_type == EXTERN_LABEL || label_type == ENTRY_LABEL){
                        pointer = strtok(NULL, " \t\n\r\f\v");
                        extern_handler(pointer,label_type); 
                    }
                    else {
                        pointer = strtok(NULL, " \t\n\r\f\v"); 
                        if (pointer == NULL){
                            error(ERR_MISSING_ARGUMENT);
                            curr_line_number++; 
                            break; 
                        }
                        define_handler(pointer,label_type);
                    }            
                    curr_line_number++;
                    break;

                /* .data .string or cmd labels */    
                case 2:
                    flag = dstring_handler(pointer); 
                    if (flag == 0){
                        curr_line_number++;
                        break;
                    }
                    else if (flag == 1){
                        curr_line_number++;
                    }
                    else if (flag == 2){
                        int len;
                        inputCopy = malloc(strlen(line) + 1); 
                        check_allocation(inputCopy);
                        strcpy(inputCopy, line);
                        len = strlen(pointer)+1;
                        full_line = inputCopy + len;
                        check_cmd(full_line);
                        curr_line_number++;
                    }
                    break;

                /* Everything else */
                case 3: 
                    check_cmd(line);
                    curr_line_number++;
                    break;
                default:
                    error(ERR_UNDEFINED_COMMAND);
                    break;
                
            }
            
        } 

        
        free(inputCopy);         
    }
    if ((IC + DC) > RAM){
        error(ERR_SIZE_LEAK);
        fclose(fp);
        return;
    }
    
}


/*
*   This function makes the required files as long as err_val = 0.
*/
void makefiles(char *clean_file_name){
    char *temp_name = NULL;  

    /* Fix the addresses in the Data segment & Labels used in Instruction segment, and lastly, make the files. */
    mergeSegments();
            
    /* Fix the entry labels to hold the correct addresses. */
    fixEntrys();

    /* Fix the commands to fill the missing information from labels. */
    fixCMDs(); 

    /* Translate the data to binary */
    translateToBin();

    /* If we have errors, dont create files, continue to translate next file given by user! */
    if (err_flag != 0){
        error(ERR_ERR_FLAG);
    }
    else {
        if (extern_head != NULL){
            /* add the correct ending to the file */
            temp_name = addFileEnding(clean_file_name,3);
            curr_file = malloc(strlen(temp_name)+1);
            check_allocation(curr_file);
            strcpy(curr_file,temp_name);
            
            /* Creating the external file */
            make_extern();
            free(curr_file);
            free(temp_name);
        }
        if (entry_head != NULL){
            /* add the correct ending to the file */
            temp_name = addFileEnding(clean_file_name,4);
            curr_file = malloc(strlen(temp_name)+1);
            check_allocation(curr_file);
            strcpy(curr_file,temp_name);

            /* Creating the entry file */
            make_entry();
            free(curr_file);
        }
        
        /* add the correct ending to the file */
        temp_name = addFileEnding(clean_file_name,2);
        curr_file = malloc(strlen(temp_name)+1);
        check_allocation(curr_file);
        strcpy(curr_file,temp_name);

        /* Creating the entry file */
        make_object();
        free(temp_name);
    }

}
