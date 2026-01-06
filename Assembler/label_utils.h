
/*
 * Header: label_utils.h
 * Description: Documented header file for PR1 project.
 */
#include "def.h"

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
int is_label_name(char* name, NODE_LB* head);

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
int legel_label_name(char* temp, MACRO* mcr_arr, int mc_size);

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

