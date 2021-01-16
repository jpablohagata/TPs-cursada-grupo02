#include "../inc/main.h"

int main(void){

	delay_t delay;
	char read[MAX_LENGTH], ssid[MAX_LENGTH], pass[MAX_LENGTH];
	uint8_t new_byte;
	uint32_t i = 0, aux = 0;

	*ssid = '\0'; *pass = '\0';
	// Inicializar la placa
	boardConfig();

	// Inicializar UART_USB
	uartConfig( UART_USB, BAUD_RATE );

	// Inicializar Retardo no bloqueante con tiempo en ms
	delayConfig( &delay, 0 );

	while(1){
		if ( delayRead( &delay ) ){
			if (uartReadByte(UART_USB, &new_byte)) {
				read[i] = (char)new_byte;
				i++;
				read[i] = '\0';

				if ((char)new_byte == '}') {
					i = 0;
					json_parse(read, ssid, pass);
				}

				uartWriteString(UART_USB, ssid);
				uartWriteString(UART_USB, " | ");
				uartWriteString(UART_USB, pass);
				uartWriteString(UART_USB, "\n");
				//uartWriteString(UART_USB, pass);

			}

			if (my_strcmp(ssid,"hola")) gpioWrite(LED1,TRUE);
			else gpioWrite(LED1,FALSE);

			if (my_strcmp(pass,"chau")) gpioWrite(LED2,TRUE);
			else gpioWrite(LED2,FALSE);
		}
	}

	return 0;
}

bool_t my_strcmp(char * s1, char * s2) {
	if (s1 == NULL) return FALSE;
	for (;*s1 != '\0';s1++, s2++) {
		if (s1 == NULL || s2 == NULL) return FALSE;
		if (*s1 != *s2) return FALSE;
	}
	if (*s2 != '\0') return FALSE;
	return TRUE;
}

void my_strcpy(char * s1, char * s2) {
	if ( s2 == NULL ) return;
	for (;*s2!='\0';s1++,s2++){
		if ( s2 == NULL ) return;
		*s1 = *s2;
	}
}

void json_parse(char * json, char * ssid, char * pass){
	char * pos[4];
	char * aux;
	uint8_t i = 0, j = 0;
	aux = json;
	for (;*aux!='\0';aux++){
		if (*aux == '"') pos[i++] = aux;
	}

	aux = json;
	for (;*aux!='\0';aux++){
		if (aux > pos[0] && aux < pos[1]) {
			*ssid = *aux;
			ssid++;
		}

		if (aux > pos[2] && aux < pos[3]) {
			*pass = *aux;
			pass++;
		}

	}


	*ssid = '\0';
	*pass = '\0';

}



