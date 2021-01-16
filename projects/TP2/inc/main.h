#ifndef __MAIN_H__
#define __MAIN_H__

#include "board.h"

typedef gpioMap_t enum
{

}gpioMap_t;

void gpioInit( gpioMap_t pin, gpioInit_t conf );
void gpioWrite( gpioMap_t pin, bool_t value );
void gpioRead( gpioMap_t pin );

#endif
