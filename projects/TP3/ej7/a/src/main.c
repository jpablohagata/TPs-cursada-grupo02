#include "main.h"

pulsador_t puslador1, puslador2, puslador3, puslador4;

int main(void)
{
	boardConfig();
	tickConfig(50);
	uartConfig( UART_232, 115200 );

	iniciar_pulsador(&puslador1, TEC1, "P1");
	iniciar_pulsador(&puslador2, TEC2, "P2");
	iniciar_pulsador(&puslador3, TEC3, "P3");
	iniciar_pulsador(&puslador4, TEC4, "P4");

	delay(500);

	while(1) {
		actualizar(&puslador1);
		actualizar(&puslador2);
		actualizar(&puslador3);
		actualizar(&puslador4);
	}

	return 0 ;
}

void iniciar_pulsador(pulsador_t * p, gpioMap_t id_pulsador, char * nombre){
	p->id_pulsador = id_pulsador;
	p->estado_pulsador = PULSADOR_OFF;
	p->nombre = nombre;
}

void actualizar(pulsador_t * p){
	switch(p->estado_pulsador){
		case PULSADOR_OFF:
			if(gpioRead(p->id_pulsador) == LOW)
				p->estado_pulsador = PULSADOR_LIBERANDO;
			break;

		case PULSADOR_ON:
			if(gpioRead(p->id_pulsador) == HIGH)
				p->estado_pulsador = PULSADOR_PUSLANDO;
			break;

		case PULSADOR_LIBERANDO:
			if(gpioRead(p->id_pulsador) == LOW){
				p->estado_pulsador = PULSADOR_ON;
				liberado(p);
			} else
				p->estado_pulsador = PULSADOR_OFF;
			break;

		case PULSADOR_PUSLANDO:
			if(gpioRead(p->id_pulsador) == HIGH){
				p->estado_pulsador = PULSADOR_OFF;
				pulsado(p);
			} else
				p->estado_pulsador = PULSADOR_ON;
			break;
	}
}

void pulsado(pulsador_t * p){
	switch(p->id_pulsador){
		case TEC1:
			gpioWrite(LED1,LOW);
			uartWriteByte( UART_232, '1');
			break;
		case TEC2:
			gpioWrite(LED2,LOW);
			uartWriteByte( UART_232, '2');
			break;
		case TEC3:
			gpioWrite(LED3,LOW);
			uartWriteByte( UART_232, '3');
			break;
		case TEC4:
			gpioWrite(LEDR,LOW);
			uartWriteByte( UART_232, 'R');
			break;
		}
}

void liberado(pulsador_t * p){
	switch(p->id_pulsador){
		case TEC1:
			gpioWrite(LED1,HIGH);
			uartWriteByte( UART_232, '1');
			break;
		case TEC2:
			gpioWrite(LED2,HIGH);
			uartWriteByte( UART_232, '2');
			break;
		case TEC3:
			gpioWrite(LED3,HIGH);
			uartWriteByte( UART_232, '3');
			break;
		case TEC4:
			gpioWrite(LEDR,HIGH);
			uartWriteByte( UART_232, 'R');
			break;
	}
}

