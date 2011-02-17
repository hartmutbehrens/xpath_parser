/*
 * parser.h
 *
 *  Created on: 26 May 2010
 *      Author: behrensh
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <time.h>



/* struct and typedef definitions */

typedef struct {
	UCSChar *data;
	struct stack_node *previous, *next, *end;
} stack_node;


/* function prototypes */

/* Check if a file exists */
int file_exists(const char *filename);

/* print values to stdout */
void print_stdout(stack_node **);

/* retrieve a wide character substring from a source string, given a starting position and length of characters to copy */
UCSChar *w_substr(const UCSChar *source,size_t , size_t );

/* given an xpath, return the text node value */
stack_node *get_text_from_xpath(VTDNav *, UCSChar *);

/* push a node onto the top of the stack */
void push_to(stack_node **,const UCSChar *);

/* pop a node value from the top of the stack */
UCSChar *pop_from(stack_node **);

UCSChar *unshift_from(stack_node **);

/*check if the stack is empty */
int is_empty(stack_node **) ;

/* print the stack */
void print_stack(stack_node *);

#endif /* PARSER_H_ */
