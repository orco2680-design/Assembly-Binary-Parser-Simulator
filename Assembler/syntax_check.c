/*
 * File: syntax_check.c
 * Description: Contains syntax validation functions for different operand types,
 *              matrix definitions, registers, numbers, and strings used in the assembler.
 */

#include "def.h"

 /*
  * Function: is_legal_mat_def
  * ---------------------------
  *  Validates a matrix definition of the form [num1][num2].
  *
  *  ERROR: pointer to the error output file
  *  word: the matrix string to validate
  *  line_num: line number in the source file
  *  num1: pointer to output first dimension
  *  num2: pointer to output second dimension
  *
  *  returns: 1 if valid, 0 otherwise
  */
int is_legal_mat_def(FILE* ERROR, char* word, int line_num, int* num1, int* num2);

/*
 * Function: is_opcode_name
 * -------------------------
 *  Checks if the given word is a valid opcode name.
 *
 *  word: the word to check
 *
 *  returns: opcode number if valid, -1 otherwise.
 */
int is_opcode_name(char* word);

/*
 * Function: is_legal_string
 * --------------------------
 *  Validates a string operand (e.g., ".string").
 *
 *  ERROR: pointer to the error output file
 *  word: string to validate
 *  line_num: current line number
 *
 *  returns: 1 if valid, 0 otherwise
 */
int is_legal_string(FILE* ERROR, char* word, int line_num);

/*
 * Function: is_instraction_name
 * ------------------------------
 *  Checks if the word is a valid instruction name.
 *
 *  word: the word to check
 *
 *  returns: instaction number if valid, -1 otherwise.
 */
int is_instraction_name(char* word);

/*
 * Function: is_register
 * ----------------------
 *  Checks if a word represents a valid register name.
 *
 *  word: the string to check
 *
 *  returns: register number if valid register, -1 otherwise.
 */
int is_register(char* word);

/*
 * Function: is_legal_number
 * --------------------------
 *  Validates that the word is a legal numeric constant.
 *
 *  word: string representing the number
 *
 *  returns: 1 if valid number, 0 otherwise
 */
int is_legal_number(char* word);

/*
 * Function: is_instant
 * ---------------------
 *  Checks if the operand is an immediate value (e.g., #number).
 *
 *  word: the operand string
 *
 *  returns: 1 if valid immediate, 0 otherwise
 */
int is_instant(char* word);

/*
 * Function: validate_matrix_indexing
 * -----------------------------------
 *  Validates the indexing of a matrix operand and extracts indexes.
 *
 *  word: full matrix operand
 *  one: buffer to store first index
 *  two: buffer to store second index
 *  ERROR: error file pointer
 *  line_num: current line number
 *
 *  returns: 1 if valid, 0 otherwise
 */
int validate_matrix_indexing(char* word, char* one, char* two, FILE* ERROR, int line_num);

/*
 * Function: get_mat_name
 * -----------------------
 *  Extracts matrix name from a full matrix operand (excluding indices).
 *
 *  word: full matrix operand
 *  name: output buffer for matrix name
 */
void get_mat_name(char* word, char* name);

/*
 * Function: check_operand
 * ------------------------
 *  Checks if an operand is a register, matrix, or label, and extracts register names if any.
 *
 *  operand: operand string to check
 *  reg1: buffer for first register name
 *  reg2: buffer for second register name
 *
 *  returns: type code of operand
 */
int check_operand(char* operand, char* reg1, char* reg2);


/*
 * Function: is_legal_mat_def
 * ---------------------------
 *  Validates a matrix definition of the form [num1][num2].
 *
 *  ERROR: pointer to the error output file
 *  word: the matrix string to validate
 *  line_num: line number in the source file
 *  num1: pointer to output first dimension
 *  num2: pointer to output second dimension
 *
 *  returns: 1 if valid, 0 otherwise
 */
int is_legal_mat_def(FILE* ERROR, char* word, int line_num, int* num1, int* num2)
{
    int i = 0, len = strlen(word);
    *num1 = 0;
    *num2 = 0;

    if (word[0] != '[')
    {
        fprintf(ERROR, "ERROR: in line %d missing opening bracket.\n", line_num+1);
        return 0;
    }

    i++;

    /* Parse first number */
    while (i < len && isdigit(word[i]))
    {
        *num1 = *num1 * 10 + (word[i] - '0');
        i++;
    }

    if (i >= len || word[i] != ']')
    {
        fprintf(ERROR, "ERROR: in line %d missing closing bracket after first number.\n", line_num+1);
        return 0;
    }

    i++;
    if (i >= len || word[i] != '[')
    {
        fprintf(ERROR, "ERROR: in line %d missing second opening bracket.\n", line_num+1);
        return 0;
    }
    i++;

    /* Parse second number */
    while (i < len && isdigit(word[i])) {
        *num2 = *num2 * 10 + (word[i] - '0');
        i++;
    }

    if (i >= len || word[i] != ']') {
        fprintf(ERROR, "ERROR: in line %d missing closing bracket after second number.\n", line_num);
        return 0;
    }

    i++;

    if (i != len) {
        fprintf(ERROR, "ERROR: in line %d extra characters after matrix definition.\n", line_num);
        return 0;
    }

    return 1;
}

/*
 * Function: is_opcode_name
 * -------------------------
 *  Checks if the given word is a valid opcode name.
 *
 *  word: the word to check
 *
 *  returns: opcode number if valid, -1 otherwise.
 */
int is_opcode_name(char* word)
{
    int i = 0;
    const char* opcodes[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr" ,"red", "prn", "rts", "stop" };
    for (i = 0; i < 16; i++)
    {
        if (!strcmp(word, opcodes[i]))
            return i;
    }
    return -1;
}

/*
 * Function: is_legal_string
 * --------------------------
 *  Validates a string operand (e.g., ".string").
 *
 *  ERROR: pointer to the error output file
 *  word: string to validate
 *  line_num: current line number
 *
 *  returns: 1 if valid, 0 otherwise
 */
int is_legal_string(FILE* ERROR, char* word, int line_num)
{
    int len;
    len = strlen(word);

    if (len < 2 || word[0] != '"' || word[len - 1] != '"')
    {
        fprintf(ERROR, "ERROR: in line %d missing or misplaced double quotes.\n", line_num+1);
        return 0;
    }

    if (len == 2)
    {
        fprintf(ERROR, "ERROR: in line %d empty string.\n", line_num+1);
        return 0;
    }

    return 1;
}

/*
 * Function: is_instraction_name
 * ------------------------------
 *  Checks if the word is a valid instruction name.
 *
 *  word: the word to check
 *
 *  returns: instaction number if valid, -1 otherwise.
 */
int is_instraction_name(char* word)
{
    int i = 0;
    const char* instractions[] = { ".data", ".string", ".mat", ".entry",".extern" };
    for (i = 0; i < 5; i++)
    {
        if (!strcmp(word, instractions[i]))
            return i;
    }
    return -1;
}

/*
 * Function: is_legal_number
 * --------------------------
 *  Validates that the word is a legal numeric constant.
 *
 *  word: string representing the number
 *
 *  returns: 1 if valid number, 0 otherwise
 */
int is_legal_number(char* word)
{
    int num;
    if (word[0] != '-' && word[0] != '+' && !(isdigit(word[0])))
        return 0;
    else
    {
        int i = 1;
        /* Validate rest of digits */
        while (word[i] != '\0')
        {
            if (!(isdigit(word[i])))
                return 0;
            else
                i++;
        }
        num = atoi(word);
        /* Check 10-bit signed limit */
        if (num > 511 || num < -512)
            return 0;
        return 1;
    }
}

/*
 * Function: is_instant
 * ---------------------
 *  Checks if the operand is an immediate value (e.g., #number).
 *
 *  word: the operand string
 *
 *  returns: 1 if valid immediate, 0 otherwise
 */
int is_instant(char* word)
{
    int i = 0;

    if (word[0] != '#')
        return 0;

    i = 1;

    if (word[i] == '+' || word[i] == '-')
        i++;

    if (!isdigit(word[i]))
        return 0;

    while (word[i] != '\0')
    {
        if (!isdigit(word[i]))
            return 0;
        i++;
    }

    if (atoi(word + 1) > 127 || atoi(word + 1) < -128)
        return 0;

    return 1;
}

/*
 * Function: is_register
 * ----------------------
 *  Checks if a word represents a valid register name.
 *
 *  word: the string to check
 *
 *  returns: register number if valid register, -1 otherwise.
 */
int is_register(char* word)
{
    int i = 0;
    const char* opcodes[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
    for (i = 0; i < 8; i++)
    {
        if (!strcmp(word, opcodes[i]))
            return i;
    }
    return -1;
}

/*
 * Function: check_operand
 * ------------------------
 *  Checks if an operand is a register, matrix, or label, and extracts register names if any.
 *
 *  operand: operand string to check
 *  reg1: buffer for first register name
 *  reg2: buffer for second register name
 *
 *  returns: type code of operand
 */
int check_operand(char* operand, char* reg1, char* reg2)
{
    if ((is_register(operand) != -1))
    {
        return register_opr; /* Register */
    }
    else if (validate_matrix_indexing(operand, reg1, reg2, NULL, 0))
    {
        return mat_opr; /* Matrix */
    }
    else if (is_instant(operand))
    {
        return instant_opr; /* Immediate */
    }
    else
    {
        return direct_opr; /* Label */
    }
}

/*
 * Function: validate_matrix_indexing
 * -----------------------------------
 *  Validates the indexing of a matrix operand and extracts indexes.
 *
 *  word: full matrix operand
 *  one: buffer to store first index
 *  two: buffer to store second index
 *  ERROR: error file pointer
 *  line_num: current line number
 *
 *  returns: 1 if valid, 0 otherwise
 */
int validate_matrix_indexing(char* word, char* one, char* two, FILE* ERROR, int line_num) {
    char temp[WORD_LEN];
    int i = 0, j = 0;
    int left_cnt = 0, right_cnt = 0;
    char c;

    while ((c = word[i]) != '\0') {
        if (c == '[') {
            if (left_cnt >= 2) {
                if (ERROR) fprintf(ERROR, "ERROR: in line %d - too many opening brackets.\n", line_num+1);
                return 0;
            }
            left_cnt++;
            j = 0;
        }
        else if (c == ']') {
            if (left_cnt - right_cnt != 1) {
                if (ERROR) fprintf(ERROR, "ERROR: in line %d - unmatched closing bracket.\n", line_num+1);
                return 0;
            }
            temp[j] = '\0';
            if (right_cnt == 0) {
                strcpy(one, temp);
            }
            else if (right_cnt == 1) {
                strcpy(two, temp);
            }
            else {
                if (ERROR) fprintf(ERROR, "ERROR: in line %d - too many indices.\n", line_num+1);
                return 0;
            }
            right_cnt++;
        }
        else {
            if (left_cnt > right_cnt) {
                if (j >= WORD_LEN - 1) {
                    if (ERROR) fprintf(ERROR, "ERROR: in line %d - index too long.\n", line_num+1);
                    return 0;
                }
                temp[j++] = c;
            }
        }
        i++;
    }

    if (left_cnt != 2 || right_cnt != 2) {
        if (ERROR) fprintf(ERROR, "ERROR: in line %d - missing or extra brackets.\n", line_num+1);
        return 0;
    }

    return 1;
}

/*
 * Function: get_mat_name
 * -----------------------
 *  Extracts matrix name from a full matrix operand (excluding indices).
 *
 *  word: full matrix operand
 *  name: output buffer for matrix name
 */
void get_mat_name(char* word, char* name)
{
    int i = 0;
    while (word[i] != '[')
    {
        name[i] = word[i];
        i++;
    }
    name[i] = '\0';
    return;
}
