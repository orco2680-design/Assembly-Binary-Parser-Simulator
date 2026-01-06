/*
 * File: parser.c
 * Description: Responsible for parsing the assembler source file.
 *              Converts input into structured lines and words, processes commas and colons, and validates formatting.
 */

#include "def.h"

 /*
  * Function: con_to_lines
  * -----------------------
  *  Reads the source file line by line and stores each line into an array.
  *
  *  f: pointer to the input file
  *  size: pointer to the number of lines read
  *
  *  returns: array of strings containing each line
  */
char** con_to_lines(FILE* f, int* size);

/* Frees a dynamically allocated array of strings */
void free_lines(char** lines, int count) ;

/*
 * Function: convert_to_words_arr
 * -------------------------------
 *  Splits an input line into individual words/tokens.
 *
 *  input: input string
 *  len: length of input string
 *  str_arr: array to hold resulting words
 *
 *  returns: number of words parsed
 */
int convert_to_words_arr(char* input, int len, char** str_arr);

/*
 * Function: chek_commas
 * -----------------------
 *  Checks the validity of comma usage in an array of words.
 *
 *  str_arr: array of words
 *  len: number of words
 *  ERROR: pointer to error output file
 *  line_num: current line number
 *
 *  returns: 1 if valid, 0 otherwise
 */
int chek_commas(char** str_arr, int len, FILE* ERROR, int line_num);

/*
 * Function: remove_commas
 * -------------------------
 *  Removes commas from an array of words and returns a new array without commas.
 *
 *  old: original array of words
 *  new: target array to hold words without commas
 *  len: number of original words
 *
 *  returns: new length of array
 */
int remove_commas(char** old, char** new, int len);

/*
 * Function: remove_colon
 * ------------------------
 *  Removes a trailing colon from a word (used for labels).
 *
 *  str: the string to modify
 */
void remove_colon(char* str);

/*
 * Function: have_comma
 * ----------------------
 *  Checks if any of the strings in the array contain commas.
 *
 *  str: array of strings
 *
 *  returns: 1 if commas are found, 0 otherwise
 */
int have_comma(char** str);


/*
  * Function: con_to_lines
  * -----------------------
  *  Reads the source file line by line and stores each line into an array.
  *
  *  f: pointer to the input file
  *  size: pointer to the number of lines read
  *
  *  returns: array of strings containing each line
  */
char** con_to_lines(FILE* f, int* size)
{
    int capacity = CAPCTY;
    int line_size = LINE_LEN;
    char buffer[LINE_LEN];
    char** lines_arr = malloc(capacity * sizeof(char*));
    if (!lines_arr)
    {
        perror("malloc failed");
        return NULL;
    }

    *size = 0;

    while (fgets(buffer, line_size, f) != NULL)
    {
        int len = strlen(buffer);
      
        if (len > 0 && buffer[len - 1] != '\n')
        {
            printf("ERROR: in line %d — line is longer than %d characters.\n", *size + 1, LINE_LEN - 1);
        }

        lines_arr[*size] = malloc(len + 1);
        if (!lines_arr[*size])
        {
            perror("malloc for line failed");
            free_lines(lines_arr, *size);
            return NULL;
            break;
        }

        strcpy(lines_arr[*size], buffer);
        (*size)++;

        if (*size >= capacity)
        {
            char** temp ;
            capacity *= 2;
            temp = realloc(lines_arr, capacity * sizeof(char*));
            if (!temp)
            {
                perror("realloc failed");
                free_lines(lines_arr, *size);
                return NULL;
                break;
            }
            lines_arr = temp;
        }
    }

    return lines_arr;
}

/*
 * Function: chek_commas
 * -----------------------
 *  Checks the validity of comma usage in an array of words.
 *
 *  str_arr: array of words
 *  len: number of words
 *  ERROR: pointer to error output file
 *  line_num: current line number
 *
 *  returns: 1 if valid, 0 otherwise
 */
int chek_commas(char** str_arr, int len, FILE* ERROR, int line_num)
{
    int i;
    int res = 1;
    int start = 0;

    if (str_arr[0][strlen(str_arr[0]) - 1] == ':')
        start++;

    if (start >= len)
        return res;

    if (strcmp(str_arr[start], ".mat") == 0)
        start++;

    if (start >= len)
        return res;

    if (str_arr[start][0] != ',')
        start++;

    if (start >= len)
        return res;

    if (str_arr[start][0] == ',')
    {
	if(start == 0)
        	fprintf(ERROR, "ERROR:in line %d Illegal leading comma.\n", line_num+1);
	else
		fprintf(ERROR, "ERROR:in line %d Illegal comma before operand.\n", line_num+1);
        return 0;
    }

    for (i = start; i < len - 1; i += 2)
    {
        if (strcmp(str_arr[i], ",") == 0)
        {
            fprintf(ERROR, "ERROR:in line %d Multiple consecutive commas.\n", line_num+1);
            return 0;
        }
        else
        {
            if (strcmp(str_arr[i + 1], ",") != 0)
            {
                fprintf(ERROR, "ERROR:in line %d Missing commas.\n", line_num+1);
                return 0;
            }
        }
    }

    if (strcmp(str_arr[len - 1], ",") == 0)
    {
        fprintf(ERROR, "ERROR:in line %d Trailing comma at end.\n", line_num+1);
        return 0;
    }

    return res;
}

/*
 * Function: remove_commas
 * -------------------------
 *  Removes commas from an array of words and returns a new array without commas.
 *
 *  old: original array of words
 *  new: target array to hold words without commas
 *  len: number of original words
 *
 *  returns: new length of array
 */
int remove_commas(char** old, char** new, int len)
{
    int i = 0, j = 0;

    for (j = 0; j < len; j++)
    {
        if (old[j] == NULL)
            continue;

        if (old[j][0] == ',')
            continue;

        new[i] = malloc(strlen(old[j]) + 1);
        if (new[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return -1;
        }
        strcpy(new[i], old[j]);
        i++;
    }
    return i;
}

/*
 * Function: convert_to_words_arr
 * -------------------------------
 *  Splits an input line into individual words/tokens.
 *
 *  input: input string
 *  len: length of input string
 *  str_arr: array to hold resulting words
 *
 *  returns: number of words parsed
 */
int convert_to_words_arr(char* input, int len, char** str_arr)
{
    int str_arr_ind = 0, input_ind = 0, word_ind = 0;

    /* Allocate space for the first word */
    str_arr[str_arr_ind] = malloc(LINE_LEN);

    while (input_ind < len)
    {
        /* Check for whitespace or comma to separate tokens */
        if (isspace(input[input_ind]) || input[input_ind] == ',')
        {
            if (word_ind)
            {
                /* End current word and prepare next */
                str_arr[str_arr_ind][word_ind] = '\0';
                str_arr_ind++;
                str_arr[str_arr_ind] = malloc(LINE_LEN);
                word_ind = 0;
            }

            /* Handle comma as a separate token */
            if (input[input_ind] == ',')
            {
                str_arr[str_arr_ind][0] = ',';
                str_arr[str_arr_ind][1] = '\0';
                str_arr_ind++;
                str_arr[str_arr_ind] = malloc(LINE_LEN);
            }
            input_ind++;
        }
        else
        {
            /* Handle quoted string (e.g., .string "abc def") */
            if (input[input_ind] == '"' && word_ind == 0)
            {
                str_arr[str_arr_ind][word_ind++] = input[input_ind++];

                /* Copy until closing quote or newline */
                while (input[input_ind] != '"' && input[input_ind] != '\n')
                {
                    str_arr[str_arr_ind][word_ind++] = input[input_ind++];
                }

                /* Add closing quote if not reached newline */
                if (input[input_ind] != '\n')
                    str_arr[str_arr_ind][word_ind++] = input[input_ind++];
            }
            else
                /* Regular character, just copy */
                str_arr[str_arr_ind][word_ind++] = input[input_ind++];
        }
    }

    /* Handle last word if it exists */
    if (word_ind)
    {
        str_arr[str_arr_ind][word_ind] = '\0';
        str_arr_ind++;
    }

    return str_arr_ind;
}

/*
 * Function: remove_colon
 * ------------------------
 *  Removes a trailing colon from a word (used for labels).
 *
 *  str: the string to modify
 */
void remove_colon(char* str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == ':')
    {
        str[len - 1] = '\0';
    }

}

/*
 * Function: have_comma
 * ----------------------
 *  Checks if any of the strings in the array contain commas.
 *
 *  str: array of strings
 *
 *  returns: 1 if commas are found, 0 otherwise
 */
int have_comma(char** str)
{
    int i = 0;
    while (str[i] && str[i][0] != '\0')
    {
        if (str[i][0] == ',')
            return 1;
        i++;
    }
    return 0;
}
/* Frees a dynamically allocated array of strings */
void free_lines(char** lines, int size) {
	int i=0;
	if (!lines) return;
	for(i=0;i<size;i++)
	{
	    free(lines[i]);
	    i++;
	}
    free(lines);
}

