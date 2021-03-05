#include "../inc/main.h"

const char * ATBOOT[] = {"AT+CWMODE=1\r\n",
		"AT+CWLAP\r\n",
		"AT+CWJAP=\"Vortex3\",\"Embebidos2021\"\r\n",
};
#define ATBOOT_MAX 3

const char * AT[] = { "AT+CIFSR\r\n",
//		"AT+CIPDNS=1,\"8.8.8.8\",\"8.8.4.4\"\r\n",
//		"AT+CIPDNS?\r\n",
//		"AT+CWDHCP?\r\n",
};
#define AT_MAX 1

const char * ATSEND[] = {
		"AT+CIPMUX=0\r\n",
//		"AT+CIPSTART=\"TCP\",\"157.92.49.38\",80\r\n",
//		"AT+CIPSTART=\"TCP\",\"http://emb.hagata.me/?temp=20&hum=0&no-out\",80\r\n",
		"AT+CIPSTART=\"TCP\",\"emb.hagata.me\",80\r\n",
		"AT+CIPSEND=59\r\n",
		"GET /?temp=20&hum=0&no-out HTTP/1.1\r\nHost:emb.hagata.me\r\n\r\n"
};
#define ATSEND_MAX 4

int main (void) {

	delay_t delayBoot, delay3s, delay5s;
	uint8_t i = 0;
	uint8_t at_id = 0, atboot_id = 0, atsend_id = 0;
	uint8_t dato = 0;
	bool_t boot = false, ready = false;

	// Inicializar la placa
	boardConfig();

	// Inicializar UART_USB
	uartConfig( UART_USB, BAUD_RATE );
	uartConfig( UART_232, BAUD_RATE );


	/*for(atboot_id = 0; atboot_id < ATBOOT_MAX; atboot_id++) {
		uartWriteString(UART_232, ATBOOT[atboot_id]);
		delay(2000);
	}

	delay(2000);*/

	// Inicializar Retardo no bloqueante con tiempo en ms
	delayConfig( &delayBoot, 3000 );


	while(1){

		gpioWrite(LED1,TRUE);

		if (!boot && delayRead(&delayBoot) ) {

			if (atboot_id == ATBOOT_MAX) {
				boot = true;
				delayConfig( &delay3s, 3000 );
				delayConfig( &delay5s, 8000 );
			}

			if (atboot_id < ATBOOT_MAX) uartWriteString(UART_232, ATBOOT[atboot_id++]);

		}

		if ( delayRead( &delay3s ) ){

			if (atsend_id == ATSEND_MAX) ready = false;
			if (ready && atsend_id < ATSEND_MAX) uartWriteString(UART_232, ATSEND[atsend_id++]);

		}

		if ( delayRead( &delay5s ) ){

			if (at_id == AT_MAX) ready = true;
			if (at_id < AT_MAX) uartWriteString(UART_232, AT[at_id++]);

		}


		if ( uartReadByte(UART_232, &dato) ) uartWriteByte(UART_USB, dato);

	}

	return 0;
}
