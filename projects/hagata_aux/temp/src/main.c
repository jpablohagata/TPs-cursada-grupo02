#include "../inc/main.h"
bool_t dht22_init();
char leer_datos_dht();

int main (void) {

	delay_t delay;
	bool_t input = false;
	bool_t aux = false;
	uint8_t i = 0;
	char dht22_dat[6];

	// Inicializar la placa
	boardConfig();

	// Inicializar UART_USB
	uartConfig( UART_USB, BAUD_RATE );

	// Inicializar Retardo no bloqueante con tiempo en ms
	delayConfig( &delay, 1000 );

	//gpioWrite(LED1,TRUE);

/*	while(!dht22_init());


	for(i=0;i<5;i++){
		uartWriteByte(UART_USB, i);
		dht22_dat[i]=leer_datos_dht();
	}

	dht22_dat[i] = '\0';
	uartWriteString(UART_USB, dht22_dat);
	uartWriteString(UART_USB, "hola");

*/

	Chip_SCU_PinMux(3,7,SCU_MODE_PULLUP | SCU_MODE_ZIF_DIS,FUNC0);
	Chip_GPIO_SetDir(LPC_GPIO_PORT,3,(1<<7),1);
	Chip_GPIO_SetValue(LPC_GPIO_PORT,3,(1<<7));
	delayInaccurateUs(1000*1000);
	Chip_GPIO_ClearValue(LPC_GPIO_PORT,3,(1<<7));
	delayInaccurateUs(1000*1000);
	Chip_GPIO_SetValue(LPC_GPIO_PORT,3,(1<<7));

	//gpioWrite(LEDB,TRUE);



	while(1){
		/*if ( delayRead( &delay ) ){
			input = Chip_GPIO_GetPinState(LPC_GPIO_PORT, 2, 7);

			if (aux) {
				delayConfig( &delay, 100 );
				aux = false;
			} else {
				delayConfig( &delay, 1000 );
				aux = true;
			}

			if (input) uartWriteString(UART_USB, "true");
			else uartWriteString(UART_USB, "false");

			uartWriteString(UART_USB, "\r\n");

		}*/
	}

	return 0;
}

bool_t dht22_init() {
	static delay_t delay;
	static bool_t step1 = false, step2 = false;
	static uint8_t dht_pin_in;

	if ( !step1 ){
		Chip_SCU_PinMux(3,7,SCU_MODE_PULLUP | SCU_MODE_ZIF_DIS,FUNC0);
		Chip_GPIO_SetDir(LPC_GPIO_PORT,3,(1<<7),1);
		delayInaccurateUs(20);
		Chip_GPIO_ClearValue(LPC_GPIO_PORT,3,(1<<7));
		delayConfig( &delay, 5 );
		step1 = true;
	}

	if ( delayRead( &delay ) && step1 && !step2 ){
		Chip_GPIO_SetValue(LPC_GPIO_PORT,3,(1<<7));
		delayInaccurateUs(30);
		Chip_SCU_PinMux(3,7,SCU_MODE_PULLUP  | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,FUNC0);
		Chip_GPIO_SetDir(LPC_GPIO_PORT,3,(1<<7),0);//dth22 como input
		dht_pin_in = Chip_GPIO_GetPinState(LPC_GPIO_PORT,3,7);
		if(dht_pin_in) gpioWrite(LED2,TRUE);
		delayInaccurateUs(80);
		dht_pin_in = Chip_GPIO_GetPinState(LPC_GPIO_PORT,3,7);
		if(!dht_pin_in) gpioWrite(LED1,TRUE);
		delayInaccurateUs(70);

		step2 = true;
		return true;
	}

	return false;
}

char leer_datos_dht(){
	uint8_t i = 0;
	char result=0;
	for (i=0; i< 8; i++) {
		//We enter this during the first start bit (low for 50uS) of the byte
		gpioWrite(LEDB,TRUE);

		while(!Chip_GPIO_GetPinState(LPC_GPIO_PORT,3,7));
		delayInaccurateUs(30);
		if (Chip_GPIO_GetPinState(LPC_GPIO_PORT,3,7)){
			result |=(1<<(7-i));
		}
		while (Chip_GPIO_GetPinState(LPC_GPIO_PORT,3,7));

	}

	return result;
}

