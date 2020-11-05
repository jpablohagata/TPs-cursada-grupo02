
#include "../../porton_levadizo/inc/main.h"

#include "../../porton_levadizo/gen/Model.h"
#include "../../porton_levadizo/inc/TimerTicks.h"


#define TICKRATE_1MS		   (1000)				/* 1000 ticks per second */
#define TICKRATE_MS			(TICKRATE_1MS)	/* 1000 ticks per second */


volatile bool SysTick_Time_Flag = false;

/*! This is a state machine */
static Model statechart;

/*! This is a timed state machine that requires timer services */
#define NOF_TIMERS (sizeof(ModelTimeEvents)/sizeof(sc_boolean))

TimerTicks ticks[NOF_TIMERS];


/*! \file This header defines prototypes for all functions that are required
 *  by the state machine implementation.
 *
 *  This is a state machine uses time events which require access to a timing
 *  service. Thus the function prototypes:
 *  - model_setTimer and
 *  - model_unsetTimer
 *  are defined.
 *
 *  This state machine makes use of operations declared in the state machines
 *  interface or internal scopes. Thus the function prototypes:
 *  - modelIface_opLED
 *  are defined.
 *
 *  These functions will be called during a 'run to completion step' (runCycle)
 *  of the statechart.
 *  There are some constraints that have to be considered for the
 *  implementation of these functions:
 *  - never call the statechart API functions from within these functions.
 *  - make sure that the execution time is as short as possible.
 */

#define LED0_R    0
#define LED0_G    1
#define LED0_B    2
#define LED1      3
#define LED2      4
#define LED3      5


/** state machine user-defined external function (action)
 *
 * @param handle state machine instance
 * @param LEDNumber number of LED
 * @param onoff state machine operation parameter
 */
void modelIface_opMotor( Model* handle, sc_integer LEDNumber, sc_boolean State )
{
	Board_LED_Set(LEDNumber, State);
}


/*! * This is a timed state machine that requires timer services
 */

/*! This function has to set up timers for the time events that are required
 *  by the state machine.
 */
/*! This function will be called for each time event that is relevant for a
 *  state when a state will be entered.
 *  \param evid An unique identifier of the event.
 *  \time_ms The time in milli seconds
 *  \periodic Indicates the the time event must be raised periodically until
 *   the timer is unset
 */
void model_setTimer( Model* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic )
{
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);
}

/*! This function has to unset timers for the time events that are required
 *  by the state machine.
 */
/*! This function will be called for each time event that is relevant for a
 *  state when a state will be left.
 *  \param evid An unique identifier of the event.
 */
void model_unsetTimer( Model* handle, const sc_eventid evid )
{
	UnsetTimerTick( ticks, NOF_TIMERS, evid );
}


/**
 * @brief	Hook on Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void){
	SysTick_Time_Flag = true;
}


/**
 * @brief	main routine for statechart example: EDU-CIAA-NXP - Model LED3
 * @return	Function should not exit.
 */
int main(void)
{
	uint32_t i;

	/* Generic Initialization */
	Board_Init();

	/* Init Ticks counter => TICKRATE_MS */
	SysTick_Config(SystemCoreClock / TICKRATE_MS);

	/* Init Timer Ticks */
	InitTimerTicks( ticks, NOF_TIMERS );

	/* Statechart Initialization */
	model_init( &statechart );
	model_enter( &statechart );

	/* LED state is toggled in the main program */
	while (1) {
		/* The uC sleeps waiting for an interruption */
		__WFI();

		/* When a interrupt wakes to the uC, the main program validates it,
		 * checking the waited Flag */
		if (SysTick_Time_Flag == true) {

			/* Then reset its Flag */
			SysTick_Time_Flag = false;

			/* Then Update all Timer Ticks */
			UpdateTimers( ticks, NOF_TIMERS );

			/* Then Scan all Timer Ticks */
			for (i = 0; i < NOF_TIMERS; i++) {

				/* Then if there are pending events */
				if (IsPendEvent( ticks, NOF_TIMERS, ticks[i].evid ) == true) {

					/* Then Raise an Event -> Ticks.evid => OK */
					model_raiseTimeEvent( &statechart, ticks[i].evid );

					/* Then Mark as Attached -> Ticks.evid => OK */
					MarkAsAttEvent( ticks, NOF_TIMERS, ticks[i].evid );
				}
			}

			/* Then Run an Cycle of Statechart */
			model_runCycle(&statechart);		// Run Cycle of Statechart
		}
	}
}


/** @} doxygen end group definition */

/*==================[end of file]============================================*/
