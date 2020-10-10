#include "stack.h"

stack_t * stack_create() {
	return NULL;
}

void stack_push( stack_t ** sp, double d ){
	stack_t * new_obj = (stack_t *)malloc( sizeof( stack_t ) );
	if( new_obj == NULL ) return;

	new_obj->data = d;
	new_obj->next = *sp;
	*sp = new_obj;
}

double stack_pop( stack_t ** sp ){
	stack_t * second_obj = (*sp)->next;
	double num = (*sp)->data;

	free( *sp );
	*sp = second_obj;
	return num;
}

void stack_destroy( struct stack_t ** sp ){
	while ( *sp != NULL ) stack_pop( sp );
}
