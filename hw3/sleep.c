/*---------------------------------------------------------------------------
  sleep.c - student file
  01/01/2016    R. Repka    This code is only used to verify your timing macros
  02/202016     R. Repka    Added include file hint
---------------------------------------------------------------------------*/
#include <unistd.h>
#include <stdio.h>
/* add other include files as necessary */
#include "Timers.h"

int main() {
	clock_t end_t;
	int delay;

	printf("Start\n");
   	/* Your macro stuff here */
	DECLARE_TIMER(timer);
	
   	/* wait for 60 seconds */
   	end_t = clock() + 60 * CLOCKS_PER_SEC;

	START_TIMER(timer);
   	while (end_t > clock()) {
		/* Consume CPU time */
    	delay = 1<<19;
    	while (delay) {
			delay--;
		}
	}
	/* more of your macro stuff */   
	STOP_TIMER(timer);

	PRINT_TIMER(timer);
	PRINT_RTIMER(timer, 2);
   
	printf("End\n");
	return 0;
}
