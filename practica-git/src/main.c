#include "main.h"

int main (void) {

	stack_t * number_stack = stack_create();
	operation_t operation;
	double result;

	// Read stdin and validate
	if ( read_stack( &number_stack, &operation ) == ERROR ) {
		printf("Ingreso no valido.\n");
		stack_destroy( &number_stack );
		return EXIT_FAILURE;
	}

	// Initialization for each case
	switch (operation) {
		case SUM: result = 0; break;
		case MULT: result = 1; break;
	}

	// Operate with the stack
	while ( number_stack != NULL ) {
		switch (operation) {
			case SUM:
				result+= stack_pop(&number_stack);
				break;

			case MULT:
				result*= stack_pop(&number_stack);
				break;
		}
	}

	// Print result
	printf( "El resultado es: %f\n", result );	

	// Free memory and exit
	stack_destroy( &number_stack );
	return EXIT_SUCCESS;
	
}

status_t read_stack( stack_t ** number_stack, operation_t * op ){
	char buffer[BUFFER_SIZE];
	unsigned stack_size = 0;
	double input = 0;
	char * ptr;

	while ( fgets( buffer, BUFFER_SIZE, stdin ) ) {
		
		input = strtod(buffer, &ptr);
		if ( strlen(ptr) > 1 ) break;

		stack_push(number_stack, input);
		
		stack_size++;
		if ( stack_size >= MAX_STACK_SIZE ) break;
	}

	// TODO: agregar diccionario y mayusculas/minusculas
	if ( strncmp(ptr, "sum", 3) == 0 ) *op = SUM;
	else if ( strncmp(ptr, "mult", 4) == 0 ) *op = MULT;
	else return ERROR;

	return SUCCESS;
}
