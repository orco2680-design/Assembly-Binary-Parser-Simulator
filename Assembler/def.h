/*
 * Header: def.h
 * Description: Documented header file for PR1 project.
 */

#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <string.h> 
#include <ctype.h>
#include <stdlib.h>

/* Maximum length of a single word in the input */
#define WORD_LEN 30

/* Initial capacity used for dynamic allocations */
#define CAPCTY 10

/* Maximum length of a single line in the source file */
#define LINE_LEN 80

/* Forward declarations for linked list structures */
typedef struct Node_LB NODE_LB;
typedef struct NODE_MMRY NODE_MMRY;

/* TYPE1 - represents memory type: Instruction Counter or Data Counter */
typedef enum TYPE1 {
    IC,
    DC
} TYPE1;

/* TYPE2 - represents the type of label: internal, external, or entry */
typedef enum TYPE2 {
    internal,
    external,
    entery  /* NOTE: "entery" seems to be a typo. Did you mean "entry"? */
} TYPE2;

/* Enumeration of supported assembly instructions */
enum {
    mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts, stop
};

/* Enumeration of assembler directives */
enum {
    data, string, mat, entry, externall
};

/* Enumeration of available registers */
enum {
    r0, r1, r2, r3, r4, r5, r6, r7
};

/* Enumeration of available operands */
enum {
    instant_opr, direct_opr, mat_opr, register_opr
};

/* Structure representing a macro definition */
typedef struct {
    char name[LINE_LEN];     /* Name of the macro */
    char* content;           /* Content of the macro */
} MACRO;

/* Structure representing a label and its associated data */
typedef struct {
    char name[LINE_LEN];     /* Name of the label */
    int memory;              /* Memory address of the label */
    TYPE2 type;              /* Type of the label (internal, external, entry) */
} LABEL;

/* Node in a linked list of labels */
struct Node_LB {
    LABEL label;             /* Label stored in the node */
    NODE_LB* next;           /* Pointer to the next node */
};

/* Structure representing a memory line in the machine code */
typedef struct {
    int memory;              /* Memory address */
    char binary[11];         /* Binary representation (10 bits + null terminator) */
    TYPE1 type;              /* Type: instruction or data */
} MEMORY_LINE;

/* Node in a linked list of memory lines */
struct NODE_MMRY {
    MEMORY_LINE mmry_line;   /* Memory line stored in the node */
    NODE_MMRY* next;         /* Pointer to the next node */
};

#endif
