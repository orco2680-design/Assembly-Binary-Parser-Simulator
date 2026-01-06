/*
 * Header: memory_utils.h
 * Description: Documented header file for PR1 project.
 */

#include "def.h"

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