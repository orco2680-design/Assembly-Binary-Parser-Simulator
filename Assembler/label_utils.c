/*
 * File: label_utils.c
 * Description: Provides utility functions for managing label definitions in the assembler.
 *              Includes label validation, memory resolution, and merging labels between instruction and data segments.
 */
 
#include "def.h"
#include "macro.h"
#include "linked_list.h"
#include "parser.h"
#include "memory_utils.h"
#include "syntax_check.h"

/*
 * Function: is_label_name
 * ------------------------
 *  Searches for a label name in the linked list and returns its memory address.
 *
 *  name: label name to search
 *  head: head of the label list
 *
 *  returns: memory address if found, -1 otherwise
 */
int is_label_name(char* name , NODE_LB* head);

/*
 * Function: is_label_type
 * ------------------------
 *  Retrieves the type of a given label name from the label list.
 *
 *  name: label name to search
 *  head: head of the label list
 *
 *  returns: label type if found, -1 otherwise
 */
int is_label_type(char* name, NODE_LB* head);

/*
 * Function: legel_label_name
 * ---------------------------
 *  Checks whether a label name is valid and not conflicting with macros.
 *
 *  temp: label name to validate
 *  mcr_arr: array of defined macros
 *  mc_size: number of macros in the array
 *
 *  returns: 1 if valid, 0 if not
 */
int legel_label_name(char* temp, MACRO* mcr_arr,int mc_size);

/*
 * Function: marge_labels
 * -----------------------
 *  Merges instruction and data label lists into a single label list.
 *
 *  lb_head: pointer to the merged label list
 *  lb_head_IC: instruction label list
 *  lb_head_DC: data label list
 *  memory_line: pointer to memory address counter
 */
void marge_labels(NODE_LB** lb_head, NODE_LB* lb_head_IC, NODE_LB* lb_head_DC, int* memory_line);

/*
 * Function: match_labels_mmry
 * ----------------------------
 *  Matches all labels to their corresponding memory addresses and updates references.
 *
 *  lb_head: label list
 *  ext_lb_head: external labels list
 *  ERROR: error output file
 *
 *  returns: 1 on success, 0 on failure
 */
int match_labels_mmry(NODE_LB** lb_head, NODE_LB** ext_lb_head, FILE* ERROR);

/*
 * Function: collect_labels
 * -------------------------
 *  Scans the input file and collects label definitions.
 *
 *  f: input file pointer
 *  ERROR: error output file pointer
 *  memory_line: pointer to memory address counter
 *  lb_head: pointer to label list
 *  ext_lb_head: pointer to external label list
 *  mcr_arr: array of macros
 *  mc_size: number of macros
 *
 *  returns: 1 on success, 0 on failure
 */
int collect_labels(FILE* f, FILE* ERROR, int* memory_line, NODE_LB** lb_head, NODE_LB** ext_lb_head, MACRO* mcr_arr, int mc_size);


/*
 * Function: is_label_name
 * ------------------------
 *  Searches for a label name in the linked list and returns its memory address.
 *
 *  name: label name to search
 *  head: head of the label list
 *
 *  returns: memory address if found, -1 otherwise
 */
int is_label_name(char* name , NODE_LB* head)
{
    NODE_LB* current = head;
    while (current != NULL)
    {
        if (strcmp(name, current->label.name) == 0)
            return current->label.memory ;
        current = current->next;
    }
    return -1;
}


/*
 * Function: is_label_type
 * ------------------------
 *  Retrieves the type of a given label name from the label list.
 *
 *  name: label name to search
 *  head: head of the label list
 *
 *  returns: label type if found, -1 otherwise
 */
int is_label_type(char* name, NODE_LB* head)
{
	if (head == NULL)
		return -1;
	else
	{
		NODE_LB* current = head;
		while (current!= NULL)
		{
			if (strcmp(name, current->label.name)==0){
				
				return current->label.type;}
			current = current->next;
		}
		return -1;
	}
}


/*
 * Function: legel_label_name
 * ---------------------------
 *  Checks whether a label name is valid and not conflicting with macros.
 *
 *  temp: label name to validate
 *  mcr_arr: array of defined macros
 *  mc_size: number of macros in the array
 *
 *  returns: 0 if valid, -1 if not
 */
int legel_label_name( char* label, MACRO* mcr_arr,int mc_size)
{
	
	if (is_opcode_name(label)!=-1)
		return -1;
	else if (is_instraction_name(label) != -1)
		return -1;
	else if (is_register(label) != -1)
		return -1;
	else if(is_mcr_name(label,mcr_arr, mc_size)!=-1)
		return -1;
	else 
	{
		int i=0;
		if(label[i]<'A' || label[i]>'z')
			return -1;
		i++;
		while(label[i]!='\0')
		{
			if(isdigit(label[i])!=0)
			{
				i++;
				continue;
			}
			else if(label[i]>='A' && label[i]<='z')
			{
				i++;
				continue;
			}	
			else
				return-1;
		}
	}
	return 0;
}


/*
 * Function: match_labels_mmry
 * ----------------------------
 *  Matches all labels to their corresponding memory addresses and updates references.
 *
 *  lb_head: label list
 *  ext_lb_head: external labels list
 *  ERROR: error output file
 *
 *  returns: 1 on success, 0 on failure
 */
int match_labels_mmry(NODE_LB** lb_head, NODE_LB** ext_lb_head, FILE* ERROR)
{
	NODE_LB* current = *lb_head;
	int res = 1;

	while (current != NULL)
	{
		if (is_label_name(current->label.name, *ext_lb_head) != -1)
		{
			current->label.type = 2;
		}
		current = current->next;
	}

	return res;
}


/*
 * Function: marge_labels
 * -----------------------
 *  Merges instruction and data label lists into a single label list.
 *
 *  lb_head: pointer to the merged label list
 *  lb_head_IC: instruction label list
 *  lb_head_DC: data label list
 *  memory_line: pointer to memory address counter
 */
void marge_labels(NODE_LB** lb_head, NODE_LB* lb_head_IC, NODE_LB* lb_head_DC, int* memory_line)
{
	NODE_LB* current;
	while (lb_head_IC)
	{
		current = createNode(&lb_head_IC->label);
		addNode(lb_head, current);
		lb_head_IC = lb_head_IC->next;
	}

	while (lb_head_DC)
	{
		current = createNode(&lb_head_DC->label);
		if (current->label.type != 1)
			current->label.memory += *(memory_line);
		addNode(lb_head, current);
		lb_head_DC = lb_head_DC->next;
	}
	
	return;
}


/*
 * Function: collect_labels
 * -------------------------
 * Performs the first pass over the assembly source file to collect label definitions
 * and allocate memory addresses for opcodes and directives.
 *
 * This function reads the file, splits it into lines, and then further into words.
 * It identifies and validates labels, checks for syntax errors, and builds two
 * linked lists of labels: one for instructions (IC) and one for data (DC).
 * At the end, both lists are merged into a single label list used for the second pass.
 *
 * Parameters:
 *   f            - Pointer to the input assembly source file.
 *   ERROR        - Pointer to the error output file.
 *   memory_line  - Pointer to the instruction counter (IC).
 *   lb_head      - Pointer to the head of the final label list.
 *   ext_lb_head  - Pointer to the head of the external label list.
 *   mcr_arr      - Array of defined macros.
 *   mc_size      - Number of macros in the macro array.
 *
 * Returns:
 *   1 if the first pass was completed successfully (no critical errors),
 *   0 or negative value if any error occurred during label collection.
 */
int collect_labels(FILE* f, FILE* ERROR, int* memory_line, NODE_LB** lb_head, NODE_LB** ext_lb_head, MACRO* mcr_arr, int mc_size)
{
	/* size: number of lines in the file
	 * res: result flag (accumulates errors)
	 * dc_mmry: data counter for data section
	 */
	int size, i = 0, res = 0, dc_mmry = 0;

	/* Convert entire file into array of lines */
	char** lines_arr = con_to_lines(f, &size);

	/* Linked list heads for IC and DC labels */
	NODE_LB* IC_head = NULL;
	NODE_LB* DC_head = NULL;

	for (i = 0; i < size; i++)
	{
		/* words_num: number of words in current line
		 * lbl_flag: indicates if label was found at beginning of line
		 */
		int words_num, lbl_flag = 0, wordnum1,j=0;

		/* Temporary arrays to store split words from the line */
		char* words_arr[LINE_LEN/2];
		char* words_arr_no_comma[LINE_LEN/4];

		/* Allocate memory for potential label found in this line */
		LABEL* temp_label = malloc(sizeof(LABEL));
		if (temp_label == NULL) 
		{
		    fprintf(stderr, "Memory allocation failed!\n");
		    return 0; 
   	 	}
		/* Split line into words, validate commas, and remove them */
		words_num = convert_to_words_arr(lines_arr[i], strlen(lines_arr[i]), words_arr);
		wordnum1 = words_num;
		res = chek_commas(words_arr, words_num, ERROR, i);
		words_num = remove_commas(words_arr, words_arr_no_comma, words_num);

		if (words_num == 0) /* Skip empty lines */
			continue;

		else if (words_arr_no_comma[0][0] == ';') /* Skip comment lines */
			continue;

		else
		{
			char first_word[WORD_LEN];
			int op = -1, instract = -1;

			/* Check if the first word is a label (ends with ':') */
			if (words_arr_no_comma[0][strlen(words_arr_no_comma[0]) - 1] == ':')
			{
				char temp[WORD_LEN];
				lbl_flag = 1;

				strcpy(temp, words_arr_no_comma[0]);
				remove_colon(temp); /* Remove ':' from label */

				/* Check the word after the label */
				strcpy(first_word, words_arr_no_comma[lbl_flag]);
				op = is_opcode_name(first_word);
				instract = is_instraction_name(first_word);

				/* Check if label already exists */
				if (is_label_name(temp, IC_head) != -1 || is_label_name(temp, DC_head) != -1)
				{
					fprintf(ERROR, "ERROR:in line %d label name taken.  \n", i+1);
					res = -1;
					continue;
				}
				/* Check for illegal label names (conflicts with macros or reserved names) */
				else if (legel_label_name(temp, mcr_arr, mc_size) == -1)
				{
					fprintf(ERROR, "ERROR:in line %d illigl label name.  \n", i+1);
					res = -1;
					continue;
				}
				else
				{
					/* Valid label  create node and add to IC or DC list */
					if (op != -1) /* Label is for opcode (instruction) */
					{
						NODE_LB* current;
						strcpy(temp_label->name, temp);
						temp_label->memory = *(memory_line);
						temp_label->type = internal;
						current = createNode(temp_label);
						addNode(&IC_head, current);
					}
					else if (instract != -1) /* Label is for directive (e.g. .data, .string) */
					{
						/* Do not add entry/external labels here */
						if (instract != entry && instract != externall)
						{
							NODE_LB* current;
							strcpy(temp_label->name, temp);
							temp_label->memory = dc_mmry;
							temp_label->type = internal;
							current = createNode(temp_label);
							addNode(&DC_head, current);
						}
						
					}
					else
					{
						/* Unknown operation or directive */
						fprintf(ERROR, "ERROR:in line %d Unknown operation.  \n", i+1);
						res = -1;
						continue;
					}
				}
			}

			/* If there was no label, or label already handled, process instruction/directive */
			strcpy(first_word, words_arr_no_comma[lbl_flag]);
			op = is_opcode_name(first_word);
			instract = is_instraction_name(first_word);

			if (op != -1)
			{
				/* Validate syntax and update IC memory */
				res += opcode_sintaxChec_mmryAlloc(ERROR, words_arr_no_comma, lbl_flag + 1, op, words_num, i, IC_head, memory_line, NULL, 0, NULL, NULL);
			}
			else if (instract != -1)
			{
				/* Validate syntax and update DC memory, handle externals */
				res += instraction_sintaxChec_mmryAlloc(ERROR, words_arr_no_comma, lbl_flag + 1, instract, words_num, i, &DC_head, &dc_mmry, ext_lb_head, 0, NULL);
			}
			else
			{
				/* Unknown operation or directive */
				fprintf(ERROR, "ERROR:in line %d Unknown operation.  \n", i+1);
				res = -1;
				continue;
			}
		}
		free(temp_label);
		while(j<wordnum1-1){
			free(words_arr[j]);j++;}
		j=0;
		while(j<words_num-1){
			free(words_arr_no_comma[j]);j++;}
	}

	/* Merge instruction and data labels into final label list */
	marge_labels(lb_head, IC_head, DC_head, memory_line);
	freeList( IC_head );
	freeList(DC_head);
	free_lines(lines_arr,size);
	
	return res;
}

