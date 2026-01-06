/*
 * Header: syntax_check.h
 * Description: Documented header file for PR1 project.
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