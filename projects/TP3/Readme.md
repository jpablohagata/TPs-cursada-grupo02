## Trabajo Practico N 3: Debounce, Uart, A/D y D/A

# Ejercicio 1

	- uartConfig( UART_USB, 115200 ): uartConfig es una macro a uartInit. Configura la UART_USB para comunicarse con un baudrate de 115200. 

	- adcConfig( ADC_ENABLE ): Configura y habilita el ADC.

	- dacConfig( DAC_ENABLE ): Configura y habilita el DAC.

	- delayConfig( &delay, 500 ): Inicializa un delay no bloqueante de periodo 500ms.

	- muestra = adcRead( CH1 ): Lee la representacion digital del valor analogico del canal 1 y lo guarda en la variable muestra.

	- uartReadByte( UART_USB, &dato ): Lee un byte (o caracter) de UART_USB y lo almacena en la variable "dato".

	- uartWriteByte( UART_USB, dato ): Escribe el caracter "dato" en la UART_USB.

	- uartWriteString( UART_USB, "ADC CH1 value: " ): Escribe el string "ADC CH1 value: " en la UART_USB.

	- dacWrite( DAC, muestra ): Escribe el valor "muestra" en el DAC.

	- uartCallbackSet(UART_USB, UART_RECEIVE, onRx, NULL): Permite la interrupcion con callback en UART_USB. El evento se lanza desde UART_RECEIVE. La funcion a ejecutar es "onRx" y no se agregan parametros adicionales (NULL).

	- uartInterrupt(UART_USB, true): Permite interrupciones en UART_USB.

