#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"

#ifndef _MAIN_H_

    #define _MAIN_H_

    #define BUFFER_SIZE 255
    #define MAX_STACK_SIZE 10

    typedef enum operation_t {
        SUM,
        MULT
    } operation_t;

    typedef enum status_t {
        SUCCESS,
        ERROR
    } status_t;

    status_t read_buffer( stack_t **, operation_t * );

#endif /*_MAIN_H_*/
