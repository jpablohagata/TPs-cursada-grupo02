#include <stdlib.h>

#ifndef _STACK_H_

	#define _STACK_H_

	typedef struct stack_t {
		double data;
		struct stack_t * next;
	} stack_t;

	stack_t * stack_create( void );
	void stack_destroy( stack_t ** );
	void stack_push( stack_t ** , double );
	double stack_pop( stack_t ** );

#endif /*_STACK_H_*/
