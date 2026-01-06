/*
 * File: memory_utils.c
 * Description: Implements memory allocation and binary encoding for assembler instructions and data.
 *              Converts instructions to binary, manages memory structure, and handles encoding for labels and constants.
 */

#include "def.h"
#include "label_utils.h"
#include "linked_list.h"
#include "syntax_check.h"

/*
 * Function: instraction_sintaxChec_mmryAlloc
 * -------------------------------------------
 *  Performs syntax checking and memory allocation for assembler instructions.
 *
 *  ERROR: pointer to error output file
 *  str_arr: array of strings containing the instruction
 *  strtI: starting index of instruction
 *  instract: instruction code
 *  words_num: number of words in the instruction
 *  line_num: current line number in source file
 *  lb_head: pointer to label list
 *  memory_line: pointer to memory address counter
 *  ext_lb_head: pointer to external label list
 *  round_num: current processing round
 *  mmry_head: pointer to memory image list
 *
 *  returns: 1 if valid and memory allocated successfully, 0 otherwise
 */
int instraction_sintaxChec_mmryAlloc(FILE* ERROR, char** str_arr, int strtI, int instract, int words_num, int line_num, NODE_LB** lb_head, int* memory_line, NODE_LB** ext_lb_head, int round_num, NODE_MMRY** mmry_head);

/*
 * Function: opcode_sintaxChec_mmryAlloc
 * -------------------------------------------
 *  Performs syntax checking and memory allocation for opcode instructions.
 *
 *  ERROR: pointer to error output file
 *  str_arr: array of strings
 *  strtI: start index
 *  op: opcode
 *  words_num: number of words
 *  line_num: current line number
 *  lb_head: pointer to label list
 *  memory_line: pointer to memory counter
 *  lb_final: final label list
 *  round_num: current round
 *  mmry_head: pointer to memory head
 *  ext_final: pointer to external label list
 *
 *  returns: 1 on success, 0 otherwise
 */
int opcode_sintaxChec_mmryAlloc(FILE* ERROR, char** str_arr, int strtI, int op, int words_num, int line_num, NODE_LB* lb_head, int* memory_line, NODE_LB* lb_final, int round_num, NODE_MMRY** mmry_head, NODE_LB** ext_final);

/*
 * Function: marge_memory
 * -----------------------
 *  Merges instruction and data memory lists into a single memory image.
 *
 *  mmry_head: final memory image list
 *  mmry_head_IC: instruction memory list
 *  mmry_head_DC: data memory list
 *  memory_line: memory counter
 */
void marge_memory(NODE_MMRY** mmry_head, NODE_MMRY* mmry_head_IC, NODE_MMRY* mmry_head_DC, int* memory_line);

/*
 * Function: IC_binery1
 * ---------------------
 *  Encodes an instruction with two operands into binary.
 *
 *  binary: output buffer for binary string
 *  opc: opcode
 *  opr1: first operand
 *  opr2: second operand
 */
void IC_binery1(char binary[11], int opc, int opr1, int opr2);

/*
 * Function: IC_binery_reg
 * ------------------------
 *  Encodes register operands into binary.
 *
 *  binary: output buffer
 *  opr1: first register
 *  opr2: second register
 */
void IC_binery_reg(char binary[11], int opr1, int opr2);

/*
 * Function: IC_binery_label
 * --------------------------
 *  Encodes label into binary based on addressing type.
 *
 *  binary: output buffer
 *  lbl: label address or value
 *  t: addressing type
 */
void IC_binery_label(char binary[11], int lbl, TYPE2 t);

/*
 * Function: IC_binery_inst
 * -------------------------
 *  Encodes an instruction with numeric value.
 *
 *  binary: output buffer
 *  num: value to encode
 */
void IC_binery_inst(char binary[11], int num);

/*
 * Function: DC_binery
 * --------------------
 *  Encodes data constant into binary format.
 *
 *  binary: output buffer
 *  num: numeric value
 */
void DC_binery(char binary[11], int num);

/*
 * Function: binary_to_decimal
 * ----------------------------
 *  Converts binary string to decimal integer.
 *
 *  binary_str: null-terminated binary string
 *
 *  returns: corresponding decimal integer
 */
int binary_to_decimal(const char* binary_str);

/*
 * Function: con_to_4_base
 * ------------------------
 *  Converts a binary number into base-4 representation.
 *
 *  bin: binary value
 *  str: output string for base-4 result
 */
void con_to_4_base(int bin, char* str);

void convert_to_binary(int op, char* binary, int bits)
{
    unsigned int mask = (1U << bits) - 1;  
    unsigned int value = (unsigned int)op & mask;
	int i;
    for ( i = 0; i < bits; i++)
    {
        binary[bits - 1 - i] = (value % 2) + '0';
        value /= 2;
    }
    binary[bits] = '\0';
}


/*
 * Function: IC_binery1
 * ---------------------
 *  Encodes an instruction with two operands into binary.
 *
 *  binary: output buffer for binary string
 *  opc: opcode
 *  opr1: first operand
 *  opr2: second operand
 */
void IC_binery1(char binary[11], int opc, int opr1, int opr2)
{
	char temp[11];
	convert_to_binary(opc, temp, 4);
	strcpy(binary, temp);
	convert_to_binary(opr1, temp, 2);
	strcat(binary, temp);
	convert_to_binary(opr2, temp, 2);
	strcat(binary, temp);
	convert_to_binary(0, temp, 2);
	strcat(binary, temp);
}

/*
 * Function: IC_binery_reg
 * ------------------------
 *  Encodes register operands into binary.
 *
 *  binary: output buffer
 *  opr1: first register
 *  opr2: second register
 */
void IC_binery_reg(char binary[11], int opr1, int opr2)
{
	char temp[11];
	convert_to_binary(opr1, temp, 4);
	strcpy(binary, temp);
	convert_to_binary(opr2, temp, 4);
	strcat(binary, temp);
	convert_to_binary(0, temp, 2);
	strcat(binary, temp);
}


/*
 * Function: IC_binery_label
 * --------------------------
 *  Encodes label into binary based on addressing type.
 *
 *  binary: output buffer
 *  lbl: label address or value
 *  t: addressing type
 */
void IC_binery_label(char binary[11],int lbl,TYPE2 t)
{
	char temp[11];
	convert_to_binary(lbl, temp, 8);
	strcpy(binary, temp);
	convert_to_binary(t, temp, 2);
	strcat(binary, temp);
}

/*
 * Function: IC_binery_inst
 * -------------------------
 *  Encodes an instruction with numeric value.
 *
 *  binary: output buffer
 *  num: value to encode
 */
void IC_binery_inst(char binary[11], int num)
{
	char temp[11];
	convert_to_binary(num, temp, 8);
	strcpy(binary, temp);
	convert_to_binary(0, temp, 2);
	strcat(binary, temp);

}

/*
 * Function: DC_binery
 * --------------------
 *  Encodes data constant into binary format.
 *
 *  binary: output buffer
 *  num: numeric value
 */
void DC_binery(char binary[11], int num)
{
	char temp[11];
	
	convert_to_binary(num, temp, 10);
	strcpy(binary, temp);
	
	
	
}

/*
 * Function: binary_to_decimal
 * ----------------------------
 *  Converts binary string to decimal integer.
 *
 *  binary_str: null-terminated binary string
 *
 *  returns: corresponding decimal integer
 */
int binary_to_decimal(const char* binary_str) 
{
	int decimal = 0,i;
	int length = strlen(binary_str);
	for ( i = 0; i < length; i++) 
	{
		if (binary_str[i] == '1') 
			decimal += (1 << (length - 1 - i)); 
	}
	return decimal;
}

/*
 * Function: con_to_4_base
 * ------------------------
 *  Converts a binary number into base-4 representation.
 *
 *  bin: binary value
 *  str: output string for base-4 result
 */
void con_to_4_base(int bin, char* str)
{
	int i = 5,temp;
	str[i--] = '\0';
	for (; i >= 0; i--)
	{
		temp = bin % 4;
		bin /= 4;
		str[i] = 'a'+ temp;
	}
	
}

/*
 * Function: marge_memory
 * -----------------------
 *  Merges instruction and data memory lists into a single memory image.
 *
 *  mmry_head: final memory image list
 *  mmry_head_IC: instruction memory list
 *  mmry_head_DC: data memory list
 *  memory_line: memory counter
 */
void marge_memory(NODE_MMRY** mmry_head, NODE_MMRY* mmry_head_IC, NODE_MMRY* mmry_head_DC, int* memory_line)
{
	NODE_MMRY* current;
	while (mmry_head_IC)
	{
		current = createNode2(&mmry_head_IC->mmry_line);
		addNode2(mmry_head, current);
		mmry_head_IC = mmry_head_IC->next;
	}

	while (mmry_head_DC)
	{
		current = createNode2(&mmry_head_DC->mmry_line);
		current->mmry_line.memory += *(memory_line);
		addNode2(mmry_head, current);
		mmry_head_DC = mmry_head_DC->next;
	}

	return;
}

/*
 * Function: instraction_sintaxChec_mmryAlloc
 * -------------------------------------------
 *  Performs syntax checking and memory allocation for assembler instructions.
 *
 *  ERROR: pointer to error output file
 *  str_arr: array of strings containing the instruction
 *  strtI: starting index of instruction
 *  instract: instruction code
 *  words_num: number of words in the instruction
 *  line_num: current line number in source file
 *  lb_head: pointer to label list
 *  memory_line: pointer to memory address counter
 *  ext_lb_head: pointer to external label list
 *  round_num: current processing round
 *  mmry_head: pointer to memory image list
 *
 *  returns: 1 if valid and memory allocated successfully, 0 otherwise
 */
int instraction_sintaxChec_mmryAlloc(FILE* ERROR, char** str_arr, int strtI, int instract, int words_num, int line_num, NODE_LB** lb_head, int* memory_line, NODE_LB** ext_lb_head, int round_num, NODE_MMRY** mmry_head)
{
	int res = 0;
	NODE_MMRY* curent;
	/* Allocate memory for temporary memory line structure */
	MEMORY_LINE* temp_mmry = malloc(sizeof(MEMORY_LINE));
	if (temp_mmry == NULL) 
		{
		    fprintf(stderr, "Memory allocation failed!\n");
		    return 0; 
   	 	}
	

	/* Set type as Data (DC) since this function handles directives */
	temp_mmry->type = DC;

	/* Handle each directive type (e.g., .data, .string, .mat, .entry, .external) */
	switch (instract)
	{
	case data:
	{
		int i = strtI;

		/* Loop over each numeric value in .data */
		while (i < words_num)
		{
			if (is_legal_number(str_arr[i]))
			{
				if (round_num)  /* Only allocate memory in second round */
				{
					DC_binery(temp_mmry->binary, atoi(str_arr[i])); /* Convert number to binary */
					temp_mmry->memory = (*memory_line);            /* Set memory address */
					curent = createNode2(temp_mmry);               /* Create memory node */
					
					addNode2(mmry_head, curent);                   /* Add to memory list */
				}
				(*memory_line)++;  /* Increment memory line counter */
			}
			else
			{
				fprintf(ERROR, "ERROR:in line %d ilegal input.\n", line_num +1);
				res = -1;
			}
			i++;
		}
		break;
	}
	case string:
	{
		int i = strtI;

		/* Check if string is valid (enclosed in quotes) */
		if (!is_legal_string(ERROR, str_arr[i], line_num))
		{
			fprintf(ERROR, "ERROR:in line %d ilegal input.\n", line_num+1);
			res = -1;
		}
		else
		{
			/* Loop over each character inside the string (excluding quotes) */
			int j = 1;
			while (j < strlen(str_arr[i]) - 1)
			{
				if (round_num)
				{
					DC_binery(temp_mmry->binary, (int)str_arr[i][j]);
					temp_mmry->memory = (*memory_line);
					curent = createNode2(temp_mmry);
					addNode2(mmry_head, curent);
				}
				(*memory_line)++;
				j++;
			}
			/* Add null terminator at the end of the string */
			if (round_num)
			{
				DC_binery(temp_mmry->binary, 0);
				temp_mmry->memory = (*memory_line);
				curent = createNode2(temp_mmry);
				addNode2(mmry_head, curent);;
			}
			(*memory_line)++;
		}
		break;
	}
	case mat:
	{
		int i = strtI, row, col;

		/* Validate matrix definition and extract dimensions */
		if (!is_legal_mat_def(ERROR, str_arr[i++], line_num, &row, &col))
		{
			res = -1;
			break;
		}
		else
		{
			/* Expecting (row * col) numeric values after the definition */
			for (; i < (strtI + 1) + (row * col); i++)
			{
				if (i >= words_num)
				{
					/* If not enough values provided, pad with 0s */
					if (round_num)
					{
						DC_binery(temp_mmry->binary, 0);
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
				else if (is_legal_number(str_arr[i]))
				{
					if (round_num)
					{

						DC_binery(temp_mmry->binary, atoi(str_arr[i]));
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
				else
				{
					fprintf(ERROR, "ERROR:in line %d ilegal input.\n", line_num+1);
					res = -1;
				}
			}
		}
		break;
	}
	case entry:
	{
		LABEL* temp_label = NULL;
		int i = strtI;
		NODE_LB* current;

		temp_label = malloc(sizeof(LABEL));
		if (temp_label == NULL) 
		{
		    fprintf(stderr, "Memory allocation failed!\n");
		    return 0; 
   	 	}

		if (round_num)
		{
			/*free*/
			free(temp_label);
			break;
		}
		if ((words_num - i) != 1)
		{
			fprintf(ERROR, "ERROR:in line %d ilegal input.\n", line_num+1);
			res = -1;
		}
		else
		{
			strcpy(temp_label->name, str_arr[i]);
			temp_label->type = entery;
			current = createNode(temp_label);
			addNode(ext_lb_head, current);
		}
		/*free*/
		free(temp_label);
		break;
	}
	case externall:
	{
		LABEL* temp_label =NULL;
		int i = strtI;
		NODE_LB* current;

		temp_label = malloc(sizeof(LABEL));
		if (temp_label == NULL) 
		{
		    fprintf(stderr, "Memory allocation failed!\n");
		    return 0; 
   	 	}

		if (round_num)
		{
			/*free*/
			free(temp_label);
			break;
		}
		if (words_num - i != 1)
		{
			fprintf(ERROR, "ERROR:in line %d ilegal input.\n", line_num+1);
			res = -1;
		}
		else
		{
			strcpy(temp_label->name, str_arr[i]);
			temp_label->type = external;
			current = createNode(temp_label);
			addNode(lb_head, current);
		}
		/*free*/
		free(temp_label);
		break;
	}
	}
	free(temp_mmry);
	return res;
}
/*
 * Function: opcode_sintaxChec_mmryAlloc
 * -------------------------------------------
 *  Performs syntax checking and memory allocation for opcode instructions.
 *
 *  ERROR: pointer to error output file
 *  str_arr: array of strings representing the parsed line
 *  strtI: start index in str_arr where operands begin
 *  op: opcode (instruction identifier)
 *  words_num: total number of words in the parsed line
 *  line_num: current line number in the source file
 *  lb_head: pointer to the head of the label list
 *  memory_line: pointer to the current memory counter
 *  lb_final: pointer to the list of final labels
 *  round_num: the current processing round (0 or 1)
 *  mmry_head: pointer to the head of the memory list
 *  ext_final: pointer to the external label list
 *
 *  returns: 1 on success, -1 on error
 */
int opcode_sintaxChec_mmryAlloc(FILE* ERROR, char** str_arr, int strtI, int op, int words_num, int line_num, NODE_LB* lb_head, int* memory_line, NODE_LB* lb_final, int round_num, NODE_MMRY** mmry_head, NODE_LB** ext_final)
{
    /* result code to return, 1 = success, -1 = error */
    int res = 0;

    /* temporary memory line to be created and populated */
    MEMORY_LINE* temp_mmry =NULL;

    /* temporary external label to be created if needed */
    LABEL* temp_ext = NULL;

    /* pointer to the current memory node being created */
    NODE_MMRY* curent;

    /* pointer to the current external label node */
    NODE_LB* current_ext;
	
	temp_mmry = malloc(sizeof(MEMORY_LINE));
	if (temp_mmry == NULL) 
		{
		    fprintf(stderr, "Memory allocation failed!\n");
		    return 0; 
   	 	}

	temp_ext = malloc(sizeof(MEMORY_LINE));
	if (temp_ext == NULL) 
		{
		    fprintf(stderr, "Memory allocation failed!\n");
		    return 0; 
   	 	}

    /* type of this memory line is always IC (instruction counter) */
    temp_mmry->type = IC;

    /* switch based on the opcode */
    switch (op)
    {
        /* case for two-operand instructions */
	case mov:
	case cmp:
	case add:
	case sub:
	case lea:
	{
		int i = strtI;

		/* Check that exactly two operands exist */
		if ((words_num - i) != 2)
		{
			fprintf(ERROR, "ERROR:in line %d incorect number of operands.\n", line_num+1);
			res = -1;
			break;
		}
		else
		{
			char one[WORD_LEN];
			char two[WORD_LEN];
			char reg1[WORD_LEN], reg2[WORD_LEN], reg3[WORD_LEN], reg4[WORD_LEN];
			int opr1, opr2;

			/* Copy both operands from the string array */
			strcpy(one, str_arr[i]);
			strcpy(two, str_arr[++i]);

			/* Determine operand types */
			opr1 = check_operand(one, reg1, reg2);
			opr2 = check_operand(two, reg3, reg4);

			if (round_num)
			{
				/* Encode the instruction (first word) */
				IC_binery1(temp_mmry->binary, op, opr1, opr2);
				temp_mmry->memory = (*memory_line);
				curent = createNode2(temp_mmry);
				addNode2(mmry_head, curent);
			}
			(*memory_line)++;

			/* If both operands are registers */
			if (opr1 == register_opr && opr2 == register_opr)
			{
				if (op == lea)
				{
					fprintf(ERROR, "ERROR:in line %d unfit operand.\n", line_num+1);
					res = -1;
					break;
				}
				if (round_num)
				{
					IC_binery_reg(temp_mmry->binary, is_register(one), is_register(two));
					temp_mmry->memory = (*memory_line);
					curent = createNode2(temp_mmry);
					addNode2(mmry_head, curent);
				}

				(*memory_line)++;
			}
			else
			{
				/* Handle first operand */
				if (opr1 == register_opr)
				{
					if (op == lea)
					{
						fprintf(ERROR, "ERROR:in line %d unfit operand.\n", line_num);
						res = -1;
						break;
					}
					if (round_num)
					{
						IC_binery_reg(temp_mmry->binary, is_register(one), 0);
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
				else if (opr1 == mat_opr)
				{
					char mat[WORD_LEN];
					get_mat_name(one, mat);

					if (is_register(reg1) == -1 || is_register(reg2) == -1)
					{
						fprintf(ERROR, "ERROR:in line %d not regester in the matrix.\n", line_num+1);
						res = -1;
					}
					if (round_num)
					{
						if (is_label_name(mat, lb_final) == -1)
						{
							fprintf(ERROR, "ERROR:in line %d Matrix name is not a saved label.\n", line_num+1);
							res = -1;
						}
						else
						{
							int typ = is_label_type(mat, lb_final);
							if (typ != 1)
								typ = 2;
							else
							{
								strcpy(temp_ext->name, mat);
								temp_ext->memory = (*memory_line);
								current_ext = createNode(temp_ext);
								addNode(ext_final, current_ext);
							}
							IC_binery_label(temp_mmry->binary, is_label_name(mat, lb_final), typ);
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
							(*memory_line)++;
							IC_binery_reg(temp_mmry->binary, is_register(reg1), is_register(reg2));
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
							(*memory_line)++;
						}
					}
					else
						(*memory_line) += 2;
				}
				else if (opr1 == direct_opr)
				{
					if (round_num)
					{
						if (is_label_name(one, lb_final) == -1)
						{
							fprintf(ERROR, "ERROR: in line %d unrecognized operand.\n", line_num+1);
							res = -1;
						}
						else
						{
							int typ = is_label_type(one, lb_final);
							if (typ != 1)
								typ = 2;
							else
							{
								strcpy(temp_ext->name, one);
								temp_ext->memory = (*memory_line);
								current_ext = createNode(temp_ext);
								addNode(ext_final, current_ext);
							}
							IC_binery_label(temp_mmry->binary, is_label_name(one, lb_final), typ);
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
						}
					}
					(*memory_line)++;
				}
				else if (opr1 == instant_opr)
				{
					if (op == lea)
					{
						fprintf(ERROR, "ERROR:in line %d unfit operand.\n", line_num+1);
						res = -1;
						break;
					}
					if (round_num)
					{
						IC_binery_inst(temp_mmry->binary, atoi(one + 1));
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}

				/* Handle second operand */
				if (opr2 == register_opr)
				{
					if (round_num)
					{
						IC_binery_reg(temp_mmry->binary, 0, is_register(two));
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
				else if (opr2 == mat_opr)
				{
					char mat[WORD_LEN];
					get_mat_name(two, mat);

					if (is_register(reg3) == -1 || is_register(reg4) == -1)
					{
						fprintf(ERROR, "ERROR:in line %d not regester in the matrix.\n", line_num+1);
						res = -1;
					}
					if (round_num)
					{
						if (is_label_name(mat, lb_final) == -1)
						{
							fprintf(ERROR, "ERROR:in line %d Matrix name is not a saved label.\n", line_num+1);
							res = -1;
						}
						else
						{
							int typ = is_label_type(mat, lb_final);
							if (typ != 1)
								typ = 2;
							else
							{
								strcpy(temp_ext->name, mat);
								temp_ext->memory = (*memory_line);
								current_ext = createNode(temp_ext);
								addNode(ext_final, current_ext);
							}
							IC_binery_label(temp_mmry->binary, is_label_name(mat, lb_final), typ);
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
							(*memory_line)++;
							IC_binery_reg(temp_mmry->binary, is_register(reg3), is_register(reg4));
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
							(*memory_line)++;
						}
					}
					else
						(*memory_line) += 2;
				}
				else if (opr2 == direct_opr)
				{
					if (round_num)
					{
						if (is_label_name(two, lb_final) == -1)
						{
							fprintf(ERROR, "ERROR: in line %d unrecognized operand.\n", line_num+1);
							res = -1;
						}
						else
						{
							int typ = is_label_type(two, lb_final);
							if (typ != 1)
								typ = 2;
							else
							{
								strcpy(temp_ext->name, two);
								temp_ext->memory = (*memory_line);
								current_ext = createNode(temp_ext);
								addNode(ext_final, current_ext);
							}
							IC_binery_label(temp_mmry->binary, is_label_name(two, lb_final), typ);
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
						}
					}
					(*memory_line)++;
				}
				else if (opr2 == instant_opr)
				{
					if (op != 1)
					{
						fprintf(ERROR, "ERROR:in line %d unfit operand.\n", line_num+1);
						res = -1;
						break;
					}
					if (round_num)
					{
						IC_binery_inst(temp_mmry->binary, atoi(one + 1));
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
			}
		}
		break;
	}

        /* case for one-operand instructions */
       	case clr:
		case not:
		case inc:
		case dec:
		case jmp:
		case bne:
		case jsr:
		case red:
		case prn:
		{
			int i = strtI;

			/* Single operand instructions require exactly 1 operand */
			if ((words_num - i) != 1)
			{
				fprintf(ERROR, "ERROR:in line %d incorect number of operands.\n", line_num+1);
				res = -1;
				break;
			}
			else
			{
				char one[WORD_LEN];
				char reg1[WORD_LEN], reg2[WORD_LEN]; /* To hold possible register names in case of matrix */
				int opr1;

				strcpy(one, str_arr[i]);

				/* Determine the addressing method of the operand */
				opr1 = check_operand(one, reg1, reg2);

				if (round_num)
				{
					/* Encode the first instruction word */
					IC_binery1(temp_mmry->binary, op, 0, opr1);
					temp_mmry->memory = (*memory_line);
					curent = createNode2(temp_mmry);
					addNode2(mmry_head, curent);
				}
				(*memory_line)++;

				if (opr1 == register_opr)
				{
					/* Operand is a register */
					if (round_num)
					{
						IC_binery_reg(temp_mmry->binary, is_register(one), 0);
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
				else if (opr1 == mat_opr)
				{
					/* Operand is a matrix reference */
					char mat[WORD_LEN];
					get_mat_name(one, mat);

					if (is_register(reg1) == -1 || is_register(reg2) == -1)
					{
						fprintf(ERROR, "ERROR:in line %d not regester in the matrix.\n", line_num+1);
						res = -1;
					}
					if (round_num)
					{
						/* Validate matrix label and encode memory */
						if (is_label_name(mat, lb_final) == -1)
						{
							fprintf(ERROR, "ERROR:in line %d Matrix name is not a saved label.\n", line_num+1);
							res = -1;
						}
						else
						{
							int typ = is_label_type(mat, lb_final);
							if (typ != 1)
								typ = 2;
							else
							{
								/* External label handling */
								strcpy(temp_ext->name, mat);
								temp_ext->memory = (*memory_line);
								current_ext = createNode(temp_ext);
								addNode(ext_final, current_ext);
							}
							IC_binery_label(temp_mmry->binary, is_label_name(mat, lb_final), typ);
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
							(*memory_line)++;

							IC_binery_reg(temp_mmry->binary, is_register(reg1), is_register(reg2));
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
							(*memory_line)++;
						}
					}
					else
						(*memory_line) += 2;
				}
				else if (opr1 == direct_opr)
				{
					/* Operand is a direct label */
					if (round_num)
					{
						if (is_label_name(one, lb_final) == -1)
						{
							fprintf(ERROR, "ERROR: in line %d unrecognized operand.\n", line_num+1);
							res = -1;
						}
						else
						{
							int typ = is_label_type(one, lb_final);
							if (typ != 1)
								typ = 2;
							else
							{
								strcpy(temp_ext->name, one);
								temp_ext->memory = (*memory_line);
								current_ext = createNode(temp_ext);
								addNode(ext_final, current_ext);
							}
							IC_binery_label(temp_mmry->binary, is_label_name(one, lb_final), typ);
							temp_mmry->memory = (*memory_line);
							curent = createNode2(temp_mmry);
							addNode2(mmry_head, curent);
						}
					}
					(*memory_line)++;
				}
				else if (opr1 == instant_opr)
				{
					/* Operand is an immediate value - allowed only for prn */
					if (op != prn)
					{
						fprintf(ERROR, "ERROR:in line %d unfit operand.\n", line_num+1);
						res = -1;
						break;
					}
					if (round_num)
					{
						IC_binery_inst(temp_mmry->binary, atoi(one + 1));
						temp_mmry->memory = (*memory_line);
						curent = createNode2(temp_mmry);
						addNode2(mmry_head, curent);
					}
					(*memory_line)++;
				}
				break;
			}
		}
		case rts:
		case stop:
		{
			int i = strtI;

			/* rts and stop must not have operands */
			if ((words_num - i) != 0)
			{
				fprintf(ERROR, "ERROR:in line %d incorect number of operands.\n", line_num+1);
				res = -1;
				break;
			}
			if (round_num)
			{
				/* Encode a simple command with no operands */
				IC_binery1(temp_mmry->binary, op, 0, 0);
				temp_mmry->memory = (*memory_line);
				curent = createNode2(temp_mmry);
				addNode2(mmry_head, curent);
			}
			(*memory_line)++;
			break;
		}
	}
	free(temp_mmry);
	free(temp_ext);
	return res;
}


