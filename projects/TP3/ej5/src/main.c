#include "main.h"


int main(void){

	static char buffer[10];
	uint16_t leer;

	boardConfig();
	uartConfig( UART_USB, 115200 );
	adcConfig( ADC_ENABLE );

	delay(500);

	while(1) {

		leer = adcRead( ADC_CH1 );

		itoa(leer, buffer, 10);

		uartWriteString( UART_USB, "ADC CH1: " );
		uartWriteString( UART_USB, buffer );
		uartWriteString( UART_USB, "\r\n" );

	}

	return 0 ;
}


char* itoa(int value, char* result, int base) {

   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );


   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

