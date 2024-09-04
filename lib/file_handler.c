#include "../include/assembler.h"



/*
*   This function adds a specific file ending to a file name.
*   @file_name - The name of the file we need to add an ending to.
*   @type - The type of ending we want.
*/
char *addFileEnding(char *file_name, int type) {
    char ending[MAX_ENDING_LEN];
    size_t file_name_len;
    size_t ending_len;
    char *result;
    
    switch (type) {
        case 0: 
            strcpy(ending,".as");
            break;
        case 1: 
            strcpy(ending,".am");
            break;
        case 2: 
            strcpy(ending,".o");
            break;
        case 3: 
            strcpy(ending,".ext");
            break;
        case 4: 
            strcpy(ending,".ent");
            break;
        default:
            return NULL; /* Return NULL for unsupported types */
    }

    file_name_len = strlen(file_name);
    ending_len = strlen(ending);

    /* Allocate memory for the new filename */
    result = malloc(file_name_len + ending_len + 1); /* Plus 1 for the null terminator */
    check_allocation(result);

    /* Copy the original filename and append the ending */
    strcpy(result, file_name);
    strcat(result, ending);
    return result;
}


/*
*   Opens a file with a specific ending in read mode.
*/
FILE *openFile(char *clean_file_name, int num) {
    char *temp_name = NULL; 
    FILE *file = NULL; 

    /* add the correct ending to the file */
    temp_name = addFileEnding(clean_file_name,num);
    curr_file = malloc(strlen(temp_name)+1);
    check_allocation(curr_file);
    strcpy(curr_file,temp_name);

        /* Try opening the file */
    file = fopen(curr_file, "r"); 
    if (file == NULL) {
        error(ERR_FILE_NAME);
        return NULL;
    }
    free(temp_name);
    return file;
}

/*
*   This function makes the .ext file as required 
*/
void make_extern(){
    label_node *label_temp = NULL;
    row_node *row_temp = NULL; 
    FILE *fp;

    /* Open the .ext file */
    fp = fopen(curr_file, "w");
    if (fp == NULL){
        error(ERR_FILE_NAME);
    }

    /* Print to the .ext file */
    else {
        label_temp = extern_head;
        while (label_temp != NULL){
            row_temp = label_temp -> row_list;
            while (row_temp != NULL){
               fprintf(fp, "%s\t%d\n", label_temp -> label_name, (row_temp -> address)); 
               row_temp = row_temp -> next_row;
            }
            label_temp = label_temp -> next_extern;
        }
    }
    fclose(fp);
}

/*
*   This function makes the .ent file as required 
*/
void make_entry(){
    label_node *label_temp = NULL;
    FILE *fp;

    /* Open the .ext file */
    fp = fopen(curr_file, "w");

    if (fp == NULL){
        error(ERR_FILE_NAME);
    }
    
    /* Print to the .ext file */
    else {
        label_temp = entry_head;
        while (label_temp != NULL){
            fprintf(fp, "%s\t%d\n", label_temp -> label_name,(label_temp -> line_init));
            label_temp = label_temp -> next_entry;
        }
    }
    
    fclose(fp);
}

/*
*   This function makes the .o file as required 
*/
void make_object(){
    cmd_node *cmd_temp = NULL;
    reg_node *reg_temp = NULL;
    data_node *data_temp = NULL;
    label_node *dc_temp = NULL;
    FILE *fp;
    char *string;                            /* 14 bits binary */
    int i;

    
    fp = fopen(curr_file, "w");
    if (fp == NULL){
        error(ERR_FILE_NAME);
    }
    else {
        fprintf(fp, "\t%d\t%d\n",(IC-1),(DC-1));
        cmd_temp = cmd_head;
        /* Add the Instruction segment to the file */
        while (cmd_temp != NULL){
            i = 0;
            fprintf(fp, "%d\t",(cmd_temp -> address));
            binToFour(fp,cmd_temp -> bin_value);
            reg_temp = cmd_temp -> next_reg;
            while (reg_temp != NULL){
                i += 1;
                fprintf(fp, "%d\t",(cmd_temp -> address)+i);
                binToFour(fp,reg_temp -> bin_value);
                reg_temp = reg_temp -> next_reg;
            }
            cmd_temp = cmd_temp -> next_cmd;
        }
        dc_temp = dc_head;
        /* Add the Data segment to the file */
        while (dc_temp != NULL){
            i = 0;
            data_temp = dc_temp -> data_node;

            /* Print the data stored */
            while (data_temp != NULL){
                fprintf(fp, "%d\t", (dc_temp -> line_init)+i);
                /* Translate the int to string and than to encrypted binary */
                string = malloc(BIN_WORD_LEN+1);
                check_allocation(string);
                add_ptr(string);
                strcpy(string,BinTranslation14Bit(data_temp -> data));
                binToFour(fp,string);
                i+=1;
                data_temp = data_temp -> next_data;
            }
            dc_temp = dc_temp -> next_dc;

        }
    } 
    fclose(fp);
}

/*
* This function fixes the addresses in entries, and also makes sure every entry has a 'partner' 
*/
void fixEntrys(){
    label_node *entry_temp = entry_head;
    label_node *label_temp = lbl_head;
    if (entry_head == NULL || lbl_head == NULL)
        return;
    while (entry_temp != NULL){
        /* If one of the lists is NULL, no need to even try searching, we dont make a file. */
        if (entry_temp -> entry_count == 0){
            curr_line_number = entry_temp -> line_init;
            error(ERR_LBL_PARTNER);
        }
        /* Find the entrys partner in the label tabel */
        while (label_temp != NULL){
            /* If we find the entrys partner, update to the correct address */
            if (strcmp(entry_temp -> label_name,label_temp->label_name) == 0 && label_temp->label_type != ENTRY_LABEL){
                entry_temp -> line_init = label_temp -> line_init;
            }
            label_temp = label_temp -> next_label;
        }
        label_temp = lbl_head;
        entry_temp = entry_temp -> next_entry;
    }
    
}   

/*
*   This function traverses through the list of commands and matches labels with correct addresses.
*   It also adds new rows for when an external appears. 
*/
void fixCMDs() {
    reg_node *reg_temp = NULL;
    cmd_node *cmd_temp = cmd_head;
    label_node *lbl_temp = lbl_head;
    int counter = 0;
    /* Traverse through the command list */
    while (cmd_temp != NULL) {
        reg_temp = cmd_temp -> next_reg;
        /* Traverse through the register list for every command node */
        while (reg_temp != NULL){
            counter++;
            /* Find the equivalent label for every register that uses one */
            if (reg_temp -> reg_type == LABEL || reg_temp -> reg_type == IMM_LBL){
                lbl_temp = findNotEntry(reg_temp -> label_name);
                /* If the label is null (it didnt find one), error. */
                if (lbl_temp == NULL){
                    curr_line_number = reg_temp -> row;
                    error(ERR_INVALID_LABEL);
                    break;
                }/* If it is an external label, we do the same thing to everybody. data = 0, ARE = 1. */
                /* If the register is an IMM LBL, it requires a bit more attention. */
                else if ( reg_temp -> reg_type == IMM_LBL){
                    /* If it is not a data or string label, than the label is not allowed to have an index, therefor its an error! */
                    if (lbl_temp -> label_type != DATA_LABEL && lbl_temp -> label_type != STRING_LABEL){
                        curr_line_number =  reg_temp -> row;
                        error(ERR_INVALID_LABEL);
                    }
                    /* If it is , we need to make sure our index does not pass the allowed boundries. */
                    else {
                        /* Compare the next immediate value with the size of the data */
                        if (reg_temp -> next_reg -> data < 0 || reg_temp -> next_reg -> data >= lbl_temp -> data_count){
                            curr_line_number =  reg_temp -> row;
                            error(ERR_SEGMENTATION_FAULT);
                            break;
                        }
                    }
                }
                if (lbl_temp -> label_type == EXTERN_LABEL){
                    add_row(lbl_temp,(cmd_temp -> address)+counter);
                    reg_temp -> data = 0;
                    reg_temp -> ARE = 1;
                }
                else {
                    reg_temp -> data = lbl_temp -> line_init;
                }
            }
            counter = 0;
            reg_temp = reg_temp -> next_reg;
        }
        cmd_temp = cmd_temp -> next_cmd;
    }
}


/* 
*   This function fixes the data segment addresses according to the last instruction.
*/
void mergeSegments(){
    label_node *dc_temp = dc_head;
    int IC_TEMP = IC+99;
    while (dc_temp != NULL){
        dc_temp -> line_init = IC_TEMP;
        IC_TEMP = IC_TEMP + (dc_temp -> data_count);
        dc_temp = dc_temp -> next_dc;
    }
}
