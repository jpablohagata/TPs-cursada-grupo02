#include "main.h"



int main(void){

	boardConfig();
	uartConfig(UART_USB, 115200);
	rgbConfig (RGB_1, LEDR, LEDG, LEDB);

	uartCallbackSet(UART_USB, UART_RECEIVE, recibir, NULL);
	uartInterrupt(UART_USB, true);

	delay(500);

	while(1) {}

	return 0 ;
}

void recibir(void * v){
	uint8_t leer = uartRxRead(UART_USB);

	switch(leer){
		case 'q':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1) + 1, rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1));
			break;
		case 'w':
			rgbWriteRaw(RGB_1, 12, rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1));
			break;
		case 'e':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1) - 1, rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1));
			break;
		case 'a':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1) + 1, rgbReadDutyBlue(RGB_1));
			break;
		case 's':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), 12, rgbReadDutyBlue(RGB_1));
			break;
		case 'd':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1) - 1, rgbReadDutyBlue(RGB_1));
			break;
		case 'z':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1) + 1);
			break;
		case 'x':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1), 12);
			break;
		case 'c':
			rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1) - 1);
			break;

	}
}
