/*
 * Header: linked_list.h
 * Description: Documented header file for PR1 project.
 */

#include "def.h"


/*
 * Function: createNode
 * ---------------------
 *  Creates a new NODE_LB node and copies the label information into it.
 *
 *  label: pointer to the LABEL structure to copy
 *
 *  returns: pointer to the newly created NODE_LB
 */
NODE_LB* createNode(LABEL* label);

/*
 * Function: addNode
 * ------------------
 *  Adds a new NODE_LB to the end of the linked list.
 *
 *  head: pointer to the head of the NODE_LB list
 *  newNode: node to be added
 */
void addNode(NODE_LB** head, NODE_LB* newNode);

/*
 * Function: freeList
 * -------------------
 *  Frees the memory allocated for the NODE_LB linked list.
 *
 *  head: pointer to the head of the list
 */
void freeList(NODE_LB* head);

/*
 * Function: printLabelList
 * -------------------------
 *  Prints all label nodes in the NODE_LB linked list.
 *
 *  head: pointer to the head of the list
 */
void printLabelList(NODE_LB* head);

/*
 * Function: createNode2
 * -----------------------
 *  Creates a new NODE_MMRY node and copies the memory line information into it.
 *
 *  mmry_line: pointer to the MEMORY_LINE structure
 *
 *  returns: pointer to the newly created NODE_MMRY
 */
NODE_MMRY* createNode2(MEMORY_LINE* mmry_line);

/*
 * Function: addNode2
 * -------------------
 *  Adds a new NODE_MMRY to the end of the memory list.
 *
 *  head: pointer to the head of the NODE_MMRY list
 *  newNode: node to be added
 */
void addNode2(NODE_MMRY** head, NODE_MMRY* newNode);

/*
 * Function: freeList2
 * --------------------
 *  Frees the memory allocated for the NODE_MMRY linked list.
 *
 *  head: pointer to the head of the list
 */
void freeList2(NODE_MMRY* head);

/*
 * Function: printMemoryList
 * --------------------------
 *  Prints all memory nodes in the NODE_MMRY linked list.
 *
 *  head: pointer to the head of the list
 */
void printMemoryList(NODE_MMRY* head);
