/*
 * File: main.c
 * Description: This is the main module of the assembler project.
 *              It handles file input, initiates processing for each file,
 *              and manages the overall flow of assembling source files.
 *				all the error meseges rifering to the ".am" file.
 */

#include "def.h"
#include "macro.h"
#include "label_utils.h"
#include "linked_list.h"
#include "parser.h"
#include "memory_utils.h"
#include "syntax_check.h"

#define BASE_MEMORY_ADDRESS 100

 /*
  * Function: file_starter
  * ----------------------------
  *  Handles the full processing of a single source file.
  *
  *  in_file: pointer to the opened source file (.as)
  *  Fname: base name of the file (without extension)
  */
void file_starter(FILE* in_file, char* Fname);

/*
 * Function: create_ent
 * ----------------------------
 *  Generates the .ent file containing all entry labels.
 *
 *  lb_head: pointer to the label list head
 *  f: file pointer to the .ent file
 *
 *  returns: 1 on success, 0 on failure
 */
int create_ent(NODE_LB** lb_head, FILE* f);

/*
 * Function: create_ext
 * ----------------------------
 *  Generates the .ext file containing all external labels.
 *
 *  lb_head: pointer to the label list head
 *  f: file pointer to the .ext file
 *
 *  returns: 1 on success, 0 on failure
 */
int create_ext(NODE_LB** lb_head, FILE* f);

/*
 * Function: create_mmry
 * ----------------------------
 *  Builds memory image from parsed source code.
 *
 *  f: input file pointer
 *  ERROR: error output file pointer
 *  memory_line: pointer to current memory address
 *  lb_head: pointer to label list head
 *  mmry_head: pointer to memory image list head
 *  ext_final: pointer to final external label list
 *
 *  returns: number of lines written to memory image
 */
int create_mmry(FILE* f, FILE* ERROR, int* memory_line, NODE_LB** lb_head, NODE_MMRY** mmry_head, NODE_LB** ext_final);

/*
 * Function: make_object_file
 * ----------------------------
 *  Generates the object file (.ob) from memory image.
 *
 *  mmry_head: pointer to memory image list head
 *  f: output file pointer
 */
void make_object_file(NODE_MMRY** mmry_head, FILE* f);

/*
 * Function: main
 * ----------------------------
 *  Entry point of the assembler program.
 *  Iterates over the provided input files, opens them, and calls file_starter to process.
 *
 *  argc: number of command-line arguments
 *  argv: array of arguments (expected to be filenames without extensions)
 *
 *  returns: 0 on successful execution
 */
int main(int argc, char* argv[])
{
	int i = 0;

	/* Check if any input files were provided */
	if (argc <= 1)
	{
		printf("ERROR: NO FILE TO PROCESS.\n");
		return 0;
	}

	/* Iterate over each provided file */
	for (i = 1; i < argc; i++)
	{
		char Fname[30];
		FILE* in_file;

		/* Build file name with .as extension */
		strcpy(Fname, argv[i]);
		strcat(Fname, ".as");

		/* Open input file */
		in_file = fopen(Fname, "r");
		if (in_file == NULL) {
    			printf("Failed to open file: %s.\n\n",Fname);
    			continue; 
		}
		printf("*Start Procesing File: %s.\n", Fname);

		/* Process file */
		file_starter(in_file, argv[i]);

		/* Close input file */
		fclose(in_file);

		printf("*Finished Procesing File: %s*\n\n", Fname);
	}
	return 0;
}


/*
 * Function: file_starter
 * ----------------------------
 *  Handles full file processing logic from preprocessor to memory/image generation.
 *
 *  in_file: opened .as file
 *  Fname: base name of the file (no extension)
 */
void file_starter(FILE* in_file, char* Fname)
{
	int arr_size, num_mcr, mmr_line = 100, res = 0;
	char** lines_arr;
	char objectf[WORD_LEN], extrnf[WORD_LEN], enteryf[WORD_LEN], asmf[WORD_LEN];
	NODE_LB* lb_head = NULL, * ext_lb_head = NULL, * ext_final = NULL;
	NODE_MMRY* mmry_head = NULL;
	MACRO* mc_arr;
	FILE* post_mcr_file, * out2, * ERROR, * extf, * entf;

	/* Build filenames for all output stages */
	sprintf(asmf, "%s.am", Fname);
	sprintf(objectf, "%s.ob", Fname);
	sprintf(enteryf, "%s.ent", Fname);
	sprintf(extrnf, "%s.ext", Fname);

	ERROR = stdout;

	/* Convert .as file to array of lines */
	lines_arr = con_to_lines(in_file, &arr_size);

	/* First pass: extract and remove macros */
	num_mcr = find_macros(lines_arr, arr_size, &mc_arr);
	
	if (num_mcr < 0)
	{
		printf("Errors founded in the preasembler stage, no files created.\n");
		return;
	}

	/* Create intermediate file without macros */
	post_mcr_file = fopen(asmf, "w+");
	create_file_no_mcro(post_mcr_file, mc_arr, lines_arr, arr_size, num_mcr);
	fclose(post_mcr_file);

	free_lines(lines_arr,arr_size);

	/* Reopen to start actual assembler passes */
	post_mcr_file = fopen(asmf, "r");

	/* First assembler pass: label collection */
	res = collect_labels(post_mcr_file, ERROR, &mmr_line, &lb_head, &ext_lb_head, mc_arr, num_mcr);

	/* Match label types (e.g., entry/external) */
	res -= match_labels_mmry(&lb_head, &ext_lb_head, ERROR);

	if (res < 0)
	{
		printf("Errors founded in the first move, no files created.\n");
		return;
	}

	/* Second assembler pass: generate memory image */
	mmr_line = 100;
	rewind(post_mcr_file);

	res += create_mmry(post_mcr_file, ERROR, &mmr_line, &lb_head, &mmry_head, &ext_final);

	if (res < 0)
	{
		printf("Errors founded in the seconde move, no files created.\n");
		return;
	}


	/* Generate object file (.ob) */
	out2 = fopen(objectf, "w+");
	make_object_file(&mmry_head, out2);

	/* Generate .ext file if needed */
	extf = fopen(extrnf, "w+");
	if (create_ext(&ext_final, extf))
		remove(extrnf);

	/* Generate .ent file if needed */
	entf = fopen(enteryf, "w+");
	if (create_ent(&lb_head, entf))
		remove(enteryf);

	/* Cleanup */
	free(mc_arr);
	freeList(ext_lb_head);
	freeList(lb_head);
	freeList2(mmry_head);
	return;
}


/*
 * Function: create_ext
 * ----------------------------
 *  Writes all external labels and their addresses to the .ext file.
 *
 *  lb_head: pointer to external labels list
 *  f: file pointer to .ext output file
 *
 *  returns: 0 if any externals were written, 1 otherwise
 */
int create_ext(NODE_LB** lb_head, FILE* f)
{
	NODE_LB* curr_old = *lb_head;
	int res = 1;

	while (curr_old)
	{
		char temp_name[WORD_LEN], temp_mmry[6], * p;
		res = 0;

		con_to_4_base(curr_old->label.memory, temp_mmry);
		strcpy(temp_name, curr_old->label.name);

		/* Remove leading 'a' padding */
		p = temp_mmry;
		while (*(p) == 'a')
		{
			p++;
		}

		fprintf(f, "%s\t%s\n", temp_name, p);
		curr_old = curr_old->next;
	}
	return res;
}


/*
 * Function: create_ent
 * ----------------------------
 *  Writes all entry labels and their addresses to the .ent file.
 *
 *  lb_head: pointer to label list
 *  f: file pointer to .ent output file
 *
 *  returns: 0 if any entries were written, 1 otherwise
 */
int create_ent(NODE_LB** lb_head, FILE* f)
{
	NODE_LB* curr_old = *lb_head;
	int res = 1;

	while (curr_old)
	{
		if (is_label_type(curr_old->label.name, *lb_head) == 2)
		{
			char temp_name[WORD_LEN], temp_mmry[6], * p;
			res = 0;

			con_to_4_base(curr_old->label.memory, temp_mmry);
			strcpy(temp_name, curr_old->label.name);

			/* Remove leading 'a' padding */
			p = temp_mmry;
			while (*(p) == 'a')
			{
				p++;
			}

			fprintf(f, "%s\t%s\n", temp_name, p);
		}
		curr_old = curr_old->next;
	}
	return res;
}


/*
 * Function: create_mmry
 * ----------------------------
 *  Second pass: parses code lines and creates memory representation (IC + DC).
 *
 *  f: pointer to opened .am file
 *  ERROR: pointer to output file for error logging
 *  memory_line: memory line counter (starting from 100)
 *  lb_head: label list pointer
 *  mmry_head: memory image output pointer
 *  ext_final: external references list pointer
 *
 *  returns: total errors encountered
 */
int create_mmry(FILE* f, FILE* ERROR, int* memory_line, NODE_LB** lb_head, NODE_MMRY** mmry_head, NODE_LB** ext_final)
{
	int size, i = 0, res = 0, dc_mmry = 0;
	char** lines_arr = con_to_lines(f, &size);
	NODE_MMRY* IC_head = NULL;
	NODE_MMRY* DC_head = NULL;

	for (i = 0; i < size; i++)
	{
		int words_num, lbl_flag = 0,wordnum1,j=0;
		char* words_arr[LINE_LEN/2];
		char* words_arr_no_comma[LINE_LEN/4];


		/* Parse line into words and remove commas */
		words_num = convert_to_words_arr(lines_arr[i], strlen(lines_arr[i]), words_arr);
		wordnum1=words_num;
		words_num = remove_commas(words_arr, words_arr_no_comma, words_num);

		if (words_num == 0)
			continue;

		else if (words_arr_no_comma[0][0] == ';')  /* comment line */
			continue;

		else
		{
			char first_word[WORD_LEN];
			int op = -1, instract = -1;

			/* Check if label is present */
			if (words_arr_no_comma[0][strlen(words_arr_no_comma[0]) - 1] == ':')
				lbl_flag = 1;

			/* Determine whether it's opcode or instruction */
			strcpy(first_word, words_arr_no_comma[lbl_flag]);
			op = is_opcode_name(first_word);
			instract = is_instraction_name(first_word);

			/* Allocate IC or DC memory accordingly */
			if (op != -1)
			{
				res += opcode_sintaxChec_mmryAlloc(ERROR, words_arr_no_comma, lbl_flag + 1, op, words_num, i, NULL, memory_line, *lb_head, 1, &IC_head, ext_final);
			}
			else if (instract != -1)
			{
				res += instraction_sintaxChec_mmryAlloc(ERROR, words_arr_no_comma, lbl_flag + 1, instract, words_num, i, NULL, &dc_mmry, NULL, 1, &DC_head);
			}
		}
		while(j<wordnum1-1){
			free(words_arr[j]);j++;}
		j=0;
		while(j<words_num-1){
			free(words_arr_no_comma[j]);j++;}
	}

	/* Merge code and data memory segments */
	marge_memory(mmry_head, IC_head, DC_head, memory_line);
	freeList2( IC_head );
	freeList2(DC_head);
	free_lines(lines_arr,size);
	return res;
}


/*
 * Function: make_object_file
 * ----------------------------
 *  Writes memory image to object (.ob) file in base 4 format.
 *
 *  mmry_head: memory list
 *  f: object file pointer
 */
void make_object_file(NODE_MMRY** mmry_head, FILE* f)
{
	FILE* temp = fopen("temp.txt", "w");
	NODE_MMRY* curr_old = *mmry_head;
	int dc_cnt = 0, ic_cnt = 0;
	char ic_4[6], dc_4[6], *q, *t;
	char line[LINE_LEN];
	while (curr_old)
	{
		char temp_bin[6], temp_mmry[6], *p;
		int curr_bin = binary_to_decimal(curr_old->mmry_line.binary);

		/* Check if line is instruction (ic) or data (dc) */
		if (curr_old->mmry_line.type == 0)
			ic_cnt++;
		else
			dc_cnt++;

		/* Convert values to base 4 representation */
		con_to_4_base(curr_bin, temp_bin);
		con_to_4_base(curr_old->mmry_line.memory, temp_mmry);

		/* Remove leading padding characters ('a') */
		p = temp_mmry;
		while (*(p) == 'a')
		{
			p++;
		}

		fprintf(temp, "%s\t%s\n", p, temp_bin);
		curr_old = curr_old->next;
	}

	con_to_4_base(ic_cnt, ic_4);
	con_to_4_base(dc_cnt, dc_4);
	q = ic_4;
	while (*(q) == 'a')
		q++;
	t = dc_4;
	while (*(t) == 'a')
		t++;

	fprintf(f, "  %s  %s\n", q, t);

	/* Close temp file and reopen it for reading */
	fclose(temp);
	temp = fopen("temp.txt", "r");
	if (!temp) {
		perror("Failed to reopen temp.txt");
		return;
	}

	/* Copy each line from temp to the object file */
	
	while (fgets(line, sizeof(line), temp)) {
		fprintf(f, "%s", line);
	}

	fclose(temp);
	remove("temp.txt");

}
