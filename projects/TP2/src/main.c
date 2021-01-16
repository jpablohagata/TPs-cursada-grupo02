#include "main.h"

void gpioInit( gpioMap_t pin, gpioInit_t conf )
{

}

void gpioWrite( gpioMap_t pin, bool_t value )
{

}

void gpioRead( gpioMap_t pin )
{

}

int main(void)
{

	//Configuro el pin del LED como salida
	gpioInit(pin_LED,output);

	while (1)
	{
		// Leo el estado del pin
		LED_status = gpioRead(pin_LED);

		// Cambio el estado del pin
		if(LED_status)
		{
			gpioWrite(pin_LED,false);
		}
		else
		{
			gpioWrite(pin_LED,true);
		}
	}

	return 0;
}
