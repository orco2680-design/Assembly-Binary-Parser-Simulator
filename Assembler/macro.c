/*
 * File: macro.c
 * Description: Handles macro processing in the assembler.
 *              Finds macro definitions, stores them in memory, and rewrites the source file without macros.
 */

#include "def.h"
#include "parser.h"
#include "syntax_check.h"

 /*
  * Function: is_mcr_name
  * ----------------------
  *  Checks whether a given word matches any defined macro name.
  *
  *  str: word to check
  *  mc_arr: array of defined macros
  *  size: number of macros
  *
  *  returns: index of macro if found, -1 otherwise
  */
int is_mcr_name(char* str, MACRO* mc_arr, int size);

/*
 * Function: find_macros
 * -----------------------
 *  Scans the lines of the source file and identifies macro definitions.
 *  Stores macros in an allocated array.
 *
 *  lines_arr: array of lines from source file
 *  size: number of lines
 *  mc_arr: pointer to array that will hold found macros
 *
 *  returns: number of macros found, or -1 on error
 */
int find_macros(char** lines_arr, int size, MACRO** mc_arr);

/*
 * Function: create_file_no_mcro
 * ------------------------------
 *  Rewrites the source file while replacing all macro calls with their content.
 *
 *  f: output file pointer
 *  mc_arr: array of defined macros
 *  lines_arr: array of input lines from the source file
 *  line_size: number of lines
 *  mcr_size: number of macros
 */
void create_file_no_mcro(FILE* f, MACRO* mc_arr, char** lines_arr, int line_size, int mcr_size);

/*
 * Function: is_mcr_name
 * ----------------------
 *  Checks whether a given word matches any defined macro name.
 *
 *  str: word to check
 *  mc_arr: array of defined macros
 *  size: number of macros
 *
 *  returns: index of macro if found, -1 otherwise
 */
int is_mcr_name(char* str, MACRO* mc_arr, int size)
{
    int i = 0;
    int res = -1;
    while (i < size)
    {
        if (strcmp(str, mc_arr[i].name) == 0)
        {
            res = i;
            break;
        }
        i++;
    }
    return res;
}

/*
 * Function: find_macros
 * -----------------------
 *  Scans the lines of the source file and identifies macro definitions.
 *  Stores macros in an allocated array.
 *
 *  lines_arr: array of lines from source file
 *  size: number of lines
 *  mc_arr: pointer to array that will hold found macros
 *
 *  returns: number of macros found, or -1 on error
 */
int find_macros(char** lines_arr, int size, MACRO** mc_arr)
{
    /* i - line index, mcro_cnt - number of found macros,
       mc_flag - flag indicating if we're inside a macro,
       capacity - current allocated macro array capacity,
       res - general result flag for error detection */
    int i = 0, mcro_cnt = 0, mc_flag = 0;
    int capacity = CAPCTY;
    int res = 0;

    /* temporary macro array allocation */
    MACRO* mc_arr_temp = malloc(capacity * sizeof(MACRO));
    if (!mc_arr_temp)
    {
        printf("malloc error\n");
        return -1;
    }

    while (i < size)
    {
        char* temp = NULL;
        int words_num, k;
        char* words_arr[LINE_LEN];

        /* split current line into words */
        words_num = convert_to_words_arr(lines_arr[i], strlen(lines_arr[i]), words_arr);

        if (words_num >= 0)
            temp = words_arr[0];

        /* macro definition detected */
        if (temp && strcmp(temp, "mcro") == 0)
        {
            /* syntax validation for macro name */
            if (have_comma(words_arr))
            {
                printf("ERROR: in line %d illegal commas.\n", i+1);
                res = -1;
            }
            else if (words_num < 2)
            {
                printf("ERROR: in line %d macro name missing.\n", i+1);
                res = -1;
            }
            else if (is_mcr_name(words_arr[1], mc_arr_temp, mcro_cnt) != -1)
            {
                printf("ERROR: in line %d macro name exist.\n", i+1);
                res = -1;
            }
            else if (is_opcode_name(words_arr[1]) != -1)
            {
                printf("ERROR: in line %d macro name is opcode name.\n", i);
                res = -1;
            }
            else
            {
                /* dynamic array resizing if needed */
                if (mcro_cnt >= capacity)
                {
                    MACRO* temp_arr;
                    capacity *= 2;
                    temp_arr = realloc(mc_arr_temp, capacity * sizeof(MACRO));
                    if (!temp_arr)
                    {
                        printf("realloc error\n");
                        free(mc_arr);
                        return -1;
                    }
                    mc_arr_temp = temp_arr;
                }

                /* initialize new macro entry */
                strcpy(mc_arr_temp[mcro_cnt].name, words_arr[1]);
                mc_arr_temp[mcro_cnt].content = malloc(1000);
                if (!mc_arr_temp[mcro_cnt].content)
                {
                    printf("malloc error\n");
                    return -1;
                }
                mc_arr_temp[mcro_cnt].content[0] = '\0';
                mc_flag = 1;
            }
        }
        /* end of macro */
        else if (temp && strcmp(temp, "mcroend") == 0)
        {
            if (mc_flag == 1)
            {
                mc_flag = 0;
                mcro_cnt++;
            }
        }
        /* macro content line */
        else if (mc_flag == 1)
        {
            strcat(mc_arr_temp[mcro_cnt].content, lines_arr[i]);
        }

        /* free temporary word array */
        for (k = 0; k < words_num; k++)
            free(words_arr[k]);

        i++;
    }

    if (res < 0)
        return res;

    /* return final macro array */
    *mc_arr = mc_arr_temp;
    return mcro_cnt;
}


/*
 * Function: create_file_no_mcro
 * ------------------------------
 *  Rewrites the source file while replacing all macro calls with their content.
 *
 *  f: output file pointer
 *  mc_arr: array of defined macros
 *  lines_arr: array of input lines from the source file
 *  line_size: number of lines
 *  mcr_size: number of macros
 */
void create_file_no_mcro(FILE* f, MACRO* mc_arr, char** lines_arr, int line_size, int mcr_size)
{
    int i = 0;
    int mcr = -1;

    while (i < line_size)
    {
        int k;
        int words_num;
        char* words_arr[WORD_LEN];

        /* allocate memory for word splitting */
        for (k = 0; k < WORD_LEN; k++) {
            words_arr[k] = malloc(WORD_LEN);
            if (!words_arr[k]) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }

        /* convert line into words */
        words_num = convert_to_words_arr(lines_arr[i], strlen(lines_arr[i]), words_arr);

        /* handle empty line */
        if (words_num < 1) {
            fprintf(f, "\n");
        }

        /* skip macro definition */
        else if (strcmp(words_arr[0], "mcro") == 0)
        {
            i++;
            while (i < line_size)
            {
                /* free and reallocate word buffer */
                for (k = 0; k < WORD_LEN; k++) free(words_arr[k]);
                for (k = 0; k < WORD_LEN; k++) {
                    words_arr[k] = malloc(WORD_LEN);
                    if (!words_arr[k]) exit(1);
                }

                words_num = convert_to_words_arr(lines_arr[i], strlen(lines_arr[i]), words_arr);
                if (words_num > 0 && strcmp(words_arr[0], "mcroend") == 0)
                    break;

                i++;
            }
        }

        /* if macro call found, print its content */
        else if ((words_num > 0) && (mcr = is_mcr_name(words_arr[0], mc_arr, mcr_size)) != -1) {
            fprintf(f, "%s", mc_arr[mcr].content);
        }

        /* otherwise, print line as-is */
        else
        {
            fprintf(f, "%s", lines_arr[i]);
        }

        /* clean up allocated memory for words */
        for (k = 0; k < WORD_LEN; k++)
            free(words_arr[k]);

        i++;
    }
}

