/*************************************************************************
* Timers.h - Instrumentation Macros Module  - Student file
*
* The timing instrumentation macros can be globally enabled by setting 
* the variable EN_TIME (ENable TIMErs) in the compiler command line, e.g., 
* use "gcc -DEN_TIME...." to enable the timers when needed.
* A compiler warning will be issued when the timers are enabled.  
* Note: All timing output should be directed to stderr.
*
*  10/08/2016   R. Repka    Fixed C++ struct warnings 
*  12/03/2016   R. Repka    Added semicolon to DECLARE_TIMER
*  09/10/2019   R. Repka    Always use time.h
*************************************************************************/
#ifndef _TIMERS_H_
#define _TIMERS_H_
#include <stdio.h>
#include <time.h>

struct timmerDetails {                                                    
    clock_t Start;    /* Start Time   - set when the timer is started */
    clock_t Stop;     /* Stop Time    - set when the timer is stopped */
    clock_t Elapsed;  /* Elapsed Time - Accumulated when the timer is stopped */
	int State;        /* Timer State  - 0=stopped / 1=running */
}; /* Elapsed Time and State must be initialized to zero */
#ifdef EN_TIME
	#define DECLARE_TIMER(A)                                                    \
		struct timmerDetails                                                    \
    		A = /* Elapsed Time and State must be initialized to zero */        \
			{                                                                   \
      		/* Start   = */ 0,                                                  \
      		/* Stop    = */ 0,                                                  \
			/* Elapsed = */ 0,                                                  \
			/* State   = */ 0,                                                  \
			}; /* Timer has been declared and defined,   ;  IS required */
      
	/* Write the timing macros here */
	#define START_TIMER(A){														\
		if (1 == A.State){														\
			fprintf(stderr, "Error, running timer "#A" started.\n");			\
		}																		\
		A.State = 1; /* Set the clock state to running */						\
		A.Start = clock(); /* Set the start time */								\
	} /* START_TIMER() */

	#define RESTART_TIMER(A){													\
		A.Elapsed = 0;	/* Reset elapsed time */								\
	} /* RESTART_TIMER() */

	#define STOP_TIMER(A){														\
		A.Stop = clock();	/* Set the stop time */								\
		if (0 == A.State){														\
			fprintf(stderr, "Error, stopped timer "#A" stopped again,\n");		\
		}																		\
		else{																	\
			A.Elapsed += A.Stop - A.Start;										\
		}																		\
		A.State = 0; /* Set state to stopped */									\
	} /* STOP_TIMER() */

	#define PRINT_TIMER(A){														\
		if (1 == A.State){														\
			STOP_TIMER(A); /* If running, stop timer */							\
		}																		\
		fprintf(stderr, "Elapsed CPU Time ("#A") = %f sec.\n", 					\
				(double)A.Elapsed / (double)CLOCKS_PER_SEC);					\
	} /* PRINT_TIMER() */

	#define PRINT_RTIMER(A,R){													\
		if (1 == A.State){														\
			STOP_TIMER(A); /* If running, stop timer */							\
		}																		\
		fprintf(stderr, "Avg Elapsed CPU Time Over %d iterations ("#A") = %f sec.\n", R, ((double)A.Elapsed / (double)CLOCKS_PER_SEC) / (double)R);					\
	} /* PRINT_RTIMER() */
	
	#define DECLARE_REPEAT_VAR(V) int _timer_##V = 0

	#define BEGIN_REPEAT_TIMING(R,V){											\
		for(_timer_##V = 0; _timer_##V < R; _timer_##V++){
	
	#define END_REPEAT_TIMING }}

#else
	/* Null macro defines */

	#define DECLARE_TIMER(A)
	#define START_TIMER(A)
	#define RESTART_TIMER(A)
	#define STOP_TIMER(A)
	#define PRINT_TIMER(A)
	#define PRINT_RTIMER(A,R)
	#define DECLARE_REPEAT_VAR(V)
	#define BEGIN_REPEAT_TIMING(R,V)
	#define END_REPEAT_TIMING

#endif /* EN_TIME */

#endif /* _TIMERS_H_ */
