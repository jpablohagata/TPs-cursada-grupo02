#include "main.h"

int main(void){

	uint8_t leer_uart;
	uint16_t leer_adc;
	static char buffer[10];
	delay_t delay1;

	boardConfig();
	uartConfig( UART_USB, 115200 );

	adcConfig( ADC_ENABLE );
	dacConfig( DAC_ENABLE );

	delayConfig( &delay1, 500 );

	while(1) {

		if(uartReadByte(UART_USB, &leer_uart)){
			convertir(leer_uart);
		}

		if ( delayRead( &delay1 ) ){

			leer_adc = adcRead( ADC_CH1 );
			itoa( leer_adc, buffer, 10 );

			uartWriteString( UART_USB, "ADC CH1: " );
			uartWriteString( UART_USB, buffer );
			uartWriteString( UART_USB, "\r\n" );
		}
	}

	return 0 ;
}

void convertir(uint8_t valor){

	switch (valor){
		case '0': dacWrite( DAC, 0); break;
		case '1': dacWrite( DAC, 114); break;
		case '2': dacWrite( DAC, 228); break;
		case '3': dacWrite( DAC, 342); break;
		case '4': dacWrite( DAC, 456); break;
		case '5': dacWrite( DAC, 570); break;
		case '6': dacWrite( DAC, 684); break;
		case '7': dacWrite( DAC, 798); break;
		case '8': dacWrite( DAC, 912); break;
		case '9': dacWrite( DAC, 1023); break;
		default: break;
	}
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
