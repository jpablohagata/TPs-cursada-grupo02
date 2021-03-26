#include "../inc/main.h"


int main (void) {

	// Conexion WiFi
	delay_t delayConn, delaySend;
	bool_t nueva_sesion = false, conectando = false, conectado = false, send_ok = true;
	char cwjap_str[MAX_LENGTH]; *cwjap_str = '\0';
	char cipsend[MAX_LENGTH]; *cipsend = '\0';
	char getreq[MAX_LENGTH]; *getreq = '\0';

	// Manejo de la conexión serie
	char read[MAX_LENGTH], ssid[MAX_LENGTH], pass[MAX_LENGTH];
	uint8_t new_byte, wifi_byte, forward;
	uint32_t i = 0;

	// Medicion de temperatura
	uint16_t temp = 0;

	// Inicializar la placa
	boardConfig();

	// Inicializar UART_USB
	uartConfig( UART_USB, BAUD_RATE );
	uartConfig( UART_232, BAUD_RATE );

	// Inicializar el ADC
	adcConfig(ADC_ENABLE);

	// Inicializar Retardo no bloqueante con tiempo en ms
	delayConfig( &delayConn, 1000 );
	delayConfig( &delaySend, 20*1000 );

	// Inicializar el modulo WiFi
	uartWriteString(UART_232, "AT+CWMODE=1\r\n");
	delay(250);

	while(1){
		// Lectura del puerto serie
		if (uartReadByte(UART_USB, &new_byte)) {
			read[i] = (char)new_byte;
			i++;
			read[i] = '\0';

			// Espero una cadena del tipo {"ssid","pass"}
			if ((char)new_byte == '}') {
				i = 0;
				wifi_parse(read, ssid, pass); // Parseo los valores leidos
				cwjap(cwjap_str, ssid, pass); // Formateo la cadena para la conexion
				nueva_sesion = true; // Aviso que hay una nueva sesion disponible
			}
		}

		// Reviso periodicamente si hay un nuevo par user/pass y pruebo conectar
		if (delayRead(&delayConn) && nueva_sesion && !conectando){
			conectando = true; nueva_sesion = false;

			gpioWrite(LED3,FALSE);

			//uartWriteString(UART_232, "AT+CWJAP=\"ssid\",\"pass\"\r\n");
			uartWriteString(UART_232, cwjap_str);
		}

		// Prendo un led si el modulo WiFi obtuvo IP correctamente (la 'K' es porque responde "OK")
		if (conectando && uartReadByte(UART_232, &wifi_byte) && wifi_byte == 'K') {
			conectando = false;
			conectado = true;
			gpioWrite(LED3,TRUE);
		}

		// Enviar los datos
		if (delayRead(&delaySend) && conectado && send_ok){

			temp = adcRead(ADC_CH1);
			temp = ajuste_analogico(temp);
			cip(temp, cipsend, getreq);

			send_ok = false;
			gpioWrite(LED1,TRUE);
			uartWriteString(UART_USB, "AT+CIPMUX=0\r\n");
			uartWriteString(UART_232, "AT+CIPMUX=0\r\n");
			delay(500);
			uartWriteString(UART_USB, "AT+CIPSTART=\"TCP\",\"emb.hagata.me\",80\r\n");
			uartWriteString(UART_232, "AT+CIPSTART=\"TCP\",\"emb.hagata.me\",80\r\n");
			delay(4000);
			//uartWriteString(UART_232, "AT+CIPSEND=59\r\n");
			uartWriteString(UART_USB, cipsend);
			uartWriteString(UART_232, cipsend);
			delay(2000);
			//uartWriteString(UART_232, "GET /?temp=20&hum=0&no-out HTTP/1.1\r\nHost:emb.hagata.me\r\n\r\n");
			uartWriteString(UART_USB, getreq);
			uartWriteString(UART_232, getreq);
			delay(3000);
			gpioWrite(LED1,FALSE);
			send_ok = true;

		}

		// Muestro por el puerto serie lo que responde el modulo WiFi
		if ( uartReadByte(UART_232, &forward) ) uartWriteByte(UART_USB, forward);

	}

	return 0;
}

// Espero un string del tipo:
// {"ssid","pass"}

void wifi_parse(char * str, char * ssid, char * pass){
	char * pos[4];
	char * aux;
	uint8_t i = 0, j = 0;
	aux = str;
	for (;*aux!='\0';aux++){
		if (*aux == '"') pos[i++] = aux;
	}

	aux = str;
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

// Comparo dos cadenas (no lo use)
bool_t my_strcmp(char * s1, char * s2) {
	if (s1 == NULL) return FALSE;
	for (;*s1 != '\0';s1++, s2++) {
		if (s1 == NULL || s2 == NULL) return FALSE;
		if (*s1 != *s2) return FALSE;
	}
	if (*s2 != '\0') return FALSE;
	return TRUE;
}

// Concateno dos cadenas y el resultado lo guardo en la primera
void my_strcat(char * out, char * in){
	char * outptr;
	char * inptr;
	// Busco el \0
	for(outptr=out;*outptr!='\0';outptr++);

	for(inptr=in;*inptr!='\0';outptr++,inptr++){
		*outptr = *inptr;
	}

	*outptr='\0';
}

// Formateo la cadena de conexion de WiFi
void cwjap(char * out, char * ssid, char * pass) {

	*out='\0';
	my_strcat(out,"AT+CWJAP=\"");
	my_strcat(out, ssid);
	my_strcat(out,"\",\"");
	my_strcat(out, pass);
	my_strcat(out,"\"\r\n");

}

// Formateo las cadenas para enviar con CIPSEND y hacer la request GET
void cip(uint16_t temp, char * send, char * get) {
	uint8_t nbytes = 57; // cantidad de bytes de la cadena a enviar
	char strbytes[4], strtemp[4];
	*send = '\0';
	*get = '\0';

	itoa(temp, strtemp);
	itoa(nbytes+my_strlen(strtemp), strbytes);

	my_strcat(send,"AT+CIPSEND=");
	my_strcat(send,strbytes);
	my_strcat(send,"\r\n");

	my_strcat(get,"GET /?temp=");
	my_strcat(get,strtemp);
	my_strcat(get,"&hum=0&no-out HTTP/1.1\r\nHost:emb.hagata.me\r\n\r\n");

}

uint16_t ajuste_analogico(uint16_t in) {
	return in/4.6;
}

// Conversion de numeros (de 0 a 999) a string
void itoa(uint16_t val, char * str) {
	const char DIC[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	uint8_t i = 0, size;

	if (val > 100) size = 3;
	else if(val<100 && val>=10) size = 2;
	else size = 1;

	str[size] = '\0';
	for (i=1;i<=size;i++) {
		str[size-i] = DIC[val%10];
		val = val/10;
	}
}

// Largo de un string
uint16_t my_strlen(char * str) {
	uint16_t i = 0;
	char * aux;

	for (aux = str; *aux != '\0'; aux++, i++);

	return i;

}



