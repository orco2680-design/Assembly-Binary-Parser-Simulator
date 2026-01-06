/*
 * Header: macro.h
 * Description: Documented header file for PR1 project.
 */

#include "def.h"

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