#include "sapi.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

#define LOREM_IPSUM2 "Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng]velit, sed quia non numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur? [33] At vero eos et accusamus et iusto odio dignissimos ducimus, qui blanditiis praesentium voluptatum deleniti atque corrupti, quos dolores et quas molestias excepturi sint, obcaecati cupiditate non provident, similique sunt in culpa, qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem rerum facilis est et expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio, cumque nihil impedit, quo minus id, quod maxime placeat, facere possimus, omnis voluptas assumenda est, omnis dolor repellendus. Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet, ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat…"

static void flushString(char ** str){
	uint8_t j = 0;
	while((*str)[j]!='\0'){
		(*str)[j++] = 0;
	}
}

static bool_t uartReadString(uint8_t readByte){
	//static uint8_t strRx[100];
	static char strRx[1024];
	static uint16_t strPos = 0;

	strRx[strPos] = readByte;
	strPos++;

	if(strPos == 1024){
		strPos = 0;
		return FALSE;
	}

	if(readByte == '\0'){
		//uartWriteByteArray(UART_USB,strRx,strPos);
		uartWriteString(UART_USB,strRx);
		strPos = 0;
		return TRUE;
	}

	return FALSE;
}


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 230400 );

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

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay1 ) ){

         /* Leo la Entrada Analogica AI0 - ADC0 CH1 */
         //muestra = adcRead( CH1 );

         /* Envío la primer parte del mnesaje a la Uart */
    	 uartWriteString( UART_USB, LOREM_IPSUM);
         uartWriteString( UART_USB, LOREM_IPSUM2);
    	 uartWriteString( UART_USB, LOREM_IPSUM);
         uartWriteString( UART_USB, LOREM_IPSUM2);
    	 uartWriteString( UART_USB, LOREM_IPSUM);
         uartWriteString( UART_USB, LOREM_IPSUM2);
         uartWriteString( UART_USB, ";\r\n" );
         /* Conversión de muestra entera a ascii con base decimal */
         //itoa( muestra, uartBuff, 10 ); /* 10 significa decimal */

         /* Enviar muestra y Enter */
         //uartWriteString( UART_USB, uartBuff );


      }

      if(delayRead(&delayRx)){
    	    uartReadByte(UART_USB,&rxData);
    	    uartReadString(rxData);

    	    object_x_pos = rxData;

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
