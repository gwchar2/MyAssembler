#include "../include/assembler.h"

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
    if (result == NULL) {
        return NULL; /* Memory allocation failed */
    }

    /* Copy the original filename and append the ending */
    strcpy(result, file_name);
    strcat(result, ending);

    return result;
}

/*
*   This function finds the first label with the same name as received, that is not an entry.
*/
void *findNotEntry(char *name){
    label_node *label_temp = NULL;
    if(lbl_head == NULL)
        return NULL;
    else {
        label_temp = lbl_head;
        while (label_temp != NULL){
            if (strcmp(label_temp->label_name,name) == 0 && label_temp -> label_type != ENTRY_LABEL)
                return label_temp;
            label_temp = label_temp -> next_label;
        }
    }
    return NULL;
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
               fprintf(fp, "%s\t%d\n", label_temp -> label_name, (row_temp -> address)+99); 
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
            fprintf(fp, "%s\t%d\n", label_temp -> label_name,(label_temp -> line_init)+99);
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
    data_node *data_temp = NULL;
    label_node *dc_temp = NULL;
    FILE *fp;
    char *string;                            /* 14 bits binary */
    int i=99;

    /* Open the .ext file */
    fp = fopen(curr_file, "w");
    if (fp == NULL){
        error(ERR_FILE_NAME);
    }
    else {
        fprintf(fp, "\t%d\t%d\n",(IC-1),(DC-1));
        cmd_temp = cmd_head;
        /* Add the Instruction segment to the file */
        /*while (cmd_temp != NULL){
            fprintf(fp, "%d\t", cmd_temp -> address);
            binToFour(fp,cmd_temp -> cmd_binary);
            L = 1;
            if (cmd_temp -> source1_binary != NULL){
               fprintf(fp, "%d\t", (cmd_temp -> address)+L);
                binToFour(fp,cmd_temp -> source1_binary);
                L+=1;
            }
            if (cmd_temp -> source2_binary != NULL){
               fprintf(fp, "%d\t", (cmd_temp -> address)+L);
                binToFour(fp,cmd_temp -> source2_binary);
                L+=1;
            }
            if (cmd_temp -> target1_binary != NULL){
                fprintf(fp, "%d\t", (cmd_temp -> address)+L);
                binToFour(fp,cmd_temp -> target1_binary);
                L+=1;
            }
            if (cmd_temp -> target2_binary != NULL){
                fprintf(fp, "%d\t", (cmd_temp -> address)+L);
                binToFour(fp,cmd_temp -> target2_binary);
                L+=1;
            }
            L=0;
            cmd_temp = cmd_temp -> next_cmd;
        }*/
        
        dc_temp = dc_head;
        /* Add the Data segment to the file */
        while (dc_temp != NULL){
            data_temp = dc_temp -> data_node;

            /* Print the data stored */
            while (data_temp != NULL){
                fprintf(fp, "%d\t", (dc_temp -> line_init)+i);
                /* Translate the int to string and than to encrypted binary */
                string = BinTranslation14Bit(data_temp -> data);
                fprintf(fp, "%d\t", data_temp -> data);
                fprintf(fp, "%s\t", string);
                binToFour(fp,string);
                i+=1;
                data_temp = data_temp -> next_data;
            }
            i=99;
            dc_temp = dc_temp -> next_label;
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
            if (entry_temp -> label_name == label_temp->label_name && label_temp->label_type != ENTRY_LABEL)
                entry_temp -> line_init = label_temp -> line_init;
            label_temp = label_temp -> next_label;
        }
        label_temp = lbl_head;
        entry_temp = entry_temp -> next_entry;
    }
}

/* 
*   This function fixes the data segment addresses according to the last instruction.
*/
void mergeSegments(){
    label_node *dc_temp = dc_head;
    int IC_TEMP = IC;
    IC_TEMP +=1;
    while (dc_temp != NULL){
        dc_temp -> line_init = IC_TEMP;
        IC_TEMP = IC_TEMP + (dc_temp -> data_count);
        dc_temp = dc_temp -> next_dc;
    }
}


void fixCMDs() {
    cmd_node *temp = cmd_head ;
    label_node *label_temp ;
    int count ,error_num ,val ;
    while (temp != NULL) {
        printf("cmd number is: %d\n", temp->cmd_num);
        
        if (temp->source1_binary != NULL) {
            if (strcmp(temp->source1_binary,"??????????????") == 0) {
                printf("source1 bin is: %s\nsource label is: %s\n",temp->source1_binary,temp->source_label);
                label_temp = findNotEntry(temp->source_label) ;
                if (label_temp == NULL)
                    error_num = ERR_INVALID_LABEL ;
                else if(label_temp-> label_type == EXTERN_LABEL) {
                    /* for extern label: value is 0, ARE is 01 */
                    strcpy(temp->source1_binary,BinTranslation12Bit(0,1)) ;
                    return ;
                }
                    else {
                        val = label_temp-> line_init ;
                        strcpy(temp->source1_binary,BinTranslation12Bit(val,2)) ;
                        return ;
                    }
            }
        }
        
        if (temp->target1_binary != NULL) {
            if (strcmp(temp->target1_binary,"??????????????") == 0) 
                printf("target1 bin is: %s\ntarget label is: %s\n",temp->target1_binary,temp->target_label);
        }
        temp = temp->next_cmd ;
    }
}
