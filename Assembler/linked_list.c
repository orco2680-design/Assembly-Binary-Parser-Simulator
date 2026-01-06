/*
 * File: linked_list.c
 * Description: Implements linked list operations for labels (NODE_LB) and memory lines (NODE_MMRY).
 *              Includes creation, insertion, deletion, and printing of list nodes.
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


/*
 * Function: createNode
 * ---------------------
 *  Creates a new NODE_LB node and copies the label information into it.
 *
 *  label: pointer to the LABEL structure to copy
 *
 *  returns: pointer to the newly created NODE_LB
 */
NODE_LB* createNode(LABEL* label)
{
    NODE_LB* newNode = (NODE_LB*)malloc(sizeof(NODE_LB));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->label.name, label->name);
    newNode->label.memory = label->memory;
    newNode->label.type = label->type;
    newNode->next = NULL;
    return newNode;
}

/*
 * Function: addNode
 * ------------------
 *  Adds a new NODE_LB to the end of the linked list.
 *
 *  head: pointer to the head of the NODE_LB list
 *  newNode: node to be added
 */

void addNode(NODE_LB** head, NODE_LB* newNode)
{
    if (*head == NULL)
        *head = newNode;
    else {
        NODE_LB* current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = newNode;
    }
}

/*
 * Function: freeList
 * -------------------
 *  Frees the memory allocated for the NODE_LB linked list.
 *
 *  head: pointer to the head of the list
 */
void freeList(NODE_LB* head)
{
    NODE_LB* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}


/*
 * Function: createNode2
 * -----------------------
 *  Creates a new NODE_MMRY node and copies the memory line information into it.
 *
 *  mmry_line: pointer to the MEMORY_LINE structure
 *
 *  returns: pointer to the newly created NODE_MMRY
 */
NODE_MMRY* createNode2(MEMORY_LINE* mmry_line)
{
    NODE_MMRY* newNode = (NODE_MMRY*)malloc(sizeof(NODE_MMRY));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->mmry_line.memory = mmry_line->memory;
    strcpy(newNode->mmry_line.binary, mmry_line->binary);
    newNode->mmry_line.type = mmry_line->type;
    newNode->next = NULL;
    return newNode;
}

/*
 * Function: addNode2
 * -------------------
 *  Adds a new NODE_MMRY to the end of the memory list.
 *
 *  head: pointer to the head of the NODE_MMRY list
 *  newNode: node to be added
 */
void addNode2(NODE_MMRY** head, NODE_MMRY* newNode)
{
    if (*head == NULL)
        *head = newNode;
    else {
        NODE_MMRY* current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = newNode;
    }
}

/*
 * Function: freeList2
 * --------------------
 *  Frees the memory allocated for the NODE_MMRY linked list.
 *
 *  head: pointer to the head of the list
 */

void freeList2(NODE_MMRY* head)
{
    NODE_MMRY* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/*
 * Function: printLabelList
 * -------------------------
 *  Prints all label nodes in the NODE_LB linked list.
 *
 *  head: pointer to the head of the list
 */
void printLabelList(NODE_LB* head)
{
    printf("Label List:\n");
    while (head != NULL) {
        printf("Name: %s, Memory: %d, Type: %d\n",
               head->label.name, head->label.memory, head->label.type);
        head = head->next;
    }
}

/*
 * Function: printMemoryList
 * --------------------------
 *  Prints all memory nodes in the NODE_MMRY linked list.
 *
 *  head: pointer to the head of the list
 */
void printMemoryList(NODE_MMRY* head)
{
    printf("Memory List:\n");
    while (head != NULL) {
        printf("Memory: %d, Binary: %s, Type: %d\n",
               head->mmry_line.memory, head->mmry_line.binary, head->mmry_line.type);
        head = head->next;
    }
}
