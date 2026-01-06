/*
 * Header: parser.h
 * Description: Documented header file for PR1 project.
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

/* Frees a dynamically allocated array of strings */
void free_lines(char** lines, int count) ;
