#include "sapi.h"

#define BAUD_RATE 115200

static void flushString(char ** str){
	uint8_t j = 0;
	while((*str)[j]!='\0'){
		(*str)[j++] = 0;
	}
}


static bool_t uartReadString(uartMap_t uart){

	static uint16_t pos = 0;
	uint8_t new_byte;
	static uint8_t packed_byte[2048];
	char new_data;

	uartReadByte(uart,&new_byte);
	packed_byte[pos++] = new_byte;
	new_data = (char)new_byte;

	if(pos == 2048){
		pos = 0;
		return FALSE;
	}

	if(new_byte == ';'){
		uartWriteByteArray(UART_USB,packed_byte,pos);
		pos = 0;
		return TRUE;
	}

	return FALSE;
}


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   volatile uint8_t new_byte;
   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, BAUD_RATE );

   /* Inicializar AnalogIO */
   /* Posibles configuraciones:
    *    ADC_ENABLE,  ADC_DISABLE,
    *    ADC_ENABLE,  ADC_DISABLE,
    */

   //adcConfig( ADC_ENABLE ); /* ADC */

   ADC_CLOCK_SETUP_T ADCSetup = {
   	       ADC_MAX_SAMPLE_RATE,   // ADC Sample rate:ADC_MAX_SAMPLE_RATE = 400KHz
   	       ADC_10BITS,            // ADC resolution: ADC_10BITS = 10
   	       DISABLE                // ADC Burst Mode: (true or false)
   	    };

   	   Chip_ADC_Init( LPC_ADC0, &ADCSetup );
   	            /* Disable burst mode */
   	           Chip_ADC_SetBurstCmd( LPC_ADC0, ENABLE );
   	            /* Set sample rate to 200KHz */
   	            Chip_ADC_SetSampleRate( LPC_ADC0, &ADCSetup, ADC_MAX_SAMPLE_RATE/2 );
   	            /* Disable all channels */
   	            Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH1, ENABLE );
   	            Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH1, ENABLE );

   	// Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH2, DISABLE );
   	//          Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH2, DISABLE );
   	//
   	//            Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH3, DISABLE );
   	//            Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH3, DISABLE );
   	//
   	//            Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH4, DISABLE );
   	//            Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH4, DISABLE );


   dacConfig( DAC_ENABLE ); /* DAC */

   /* Configuración de estado inicial del Led */
   bool_t ledState1 = OFF;

   /* Contador */
   uint32_t i = 0;
   uint8_t rxData;
   int16_t object_x_pos = 100;
   int16_t object_y_pos = -20;

   /* Buffer */
   static char uartBuff[10];

   /* Variable para almacenar el valor leido del ADC CH1 */
   uint16_t muestra = 0;

   /* Variables de delays no bloqueantes */
   delay_t delay1, delay2, delayRx;

   /* Inicializar Retardo no bloqueante con tiempo en ms */
   delayConfig( &delay1, 0 );
   delayConfig( &delay2, 200 );
   delayConfig( &delayRx, 0);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      if ( delayRead( &delay1 ) ){

    	 //uartReadString( UART_USB);
    	 uartReadByte(UART_USB,&new_byte);
    	 uartWriteByte(UART_USB,new_byte);
      }

      if(delayRead(&delayRx)){

    	    //object_x_pos = rxData;
    		if(object_x_pos > 0){
    			gpioWrite(LED1,FALSE);
    			gpioWrite(LEDB,TRUE);
    		}else if(object_x_pos <= 0){
    			gpioWrite(LEDB,FALSE);
    			gpioWrite(LED1,TRUE);
    		}

    		if(object_y_pos > 0){
    			gpioWrite(LED3,FALSE);
				gpioWrite(LED2,TRUE);
    		}else if(object_y_pos <= 0){
    			gpioWrite(LED2,FALSE);
    			gpioWrite(LED3,TRUE);
    		}
      }
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
