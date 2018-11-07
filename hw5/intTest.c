/**************************************************************************
  intTest.c - for students
  This program demonstrates the speed advantages of integer versus floating
  point math.
 12/28/2015 - R. Repka  Initial version
 10/12/2016 - R. Repka  Switched to PRINT_RTIMER() macro
 03/25/2018 - R. Repka  Switched to timing macros 
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"

/****************************************************************************
  Use this to define the number of iterations in your performance measurements
****************************************************************************/
#define ITERATIONS	(10000)
#define FIX_2		((2 << QN))

/* Optional verbose flag, compile with -DVERBOSE or not */
#ifdef VERBOSE
   #undef VERBOSE 
   #define VERBOSE (1)
#else 
   #define VERBOSE (0)
#endif

/****************************************************************************
  Remember to set the QN value in rootfinding.h
****************************************************************************/

/************************************************************************
   Simple code to test Qn performance vs floating point  
************************************************************************/
int main(int argc, char* argv[]) {
	double Result;		  /* The result from floating point calculation   */
	long  iResult;		  /* The result from floating point calculation   */
	
	/* The bisection guesses and tolerance value						*/
	double a = -25.0;
	double b = 25.0;
	double tol= 0.000001;

	double ia = FLOAT_TO_FIX(a);
	double ib = FLOAT_TO_FIX(b);
	double itol = FLOAT_TO_FIX(tol);

	/* You many need to modify the timing code for your macro implementation! */
	DECLARE_TIMER(timer);
	DECLARE_REPEAT_VAR(tloop);

	/************************************************************************
	 Run the Qn format test 
	************************************************************************/
	/* You many need to modify the timing code for your macro implementation! */
	START_TIMER(timer);
	BEGIN_REPEAT_TIMING(ITERATIONS,tloop)
		 iResult = ibisection(&iline, ia, ib, itol, 0);
	END_REPEAT_TIMING
	STOP_TIMER(timer);

	if(iNAN == iResult) {
		fprintf(stdout, "Qn %d bisection couldn't find a root. \n", QN);
		exit(99);
	}
	else {
		fprintf(stdout, "Found the root %f using Qn %d bisection.\n",
						FIX_TO_FLOAT(iResult), QN);
	}
	fflush(stdout);
	PRINT_TIMER(timer);
	PRINT_RTIMER(timer, ITERATIONS);

	/************************************************************************
	 Run the floating point format test 
	************************************************************************/
	RESTART_TIMER(timer);
	START_TIMER(timer);
	BEGIN_REPEAT_TIMING(ITERATIONS,tloop)
		Result = bisection(&rline, a, b, tol, 0);
	END_REPEAT_TIMING
	STOP_TIMER(timer);

	if(NAN == Result) {
		fprintf(stdout, "Float bisection couldn't find a root. \n");
		exit(99);
	}
	else {
		fprintf(stdout, "Found the root %f using float bisection.\n",
						Result);
	}
	fflush(stdout);
	PRINT_TIMER(timer);
	PRINT_RTIMER(timer, ITERATIONS);
	
	/* Display the percentage error */
	Result = (FIX_TO_FLOAT(iResult) - Result)/Result;
	fprintf(stdout, "Qn %d error= %f\n", QN, Result);
	return 0;
} /* End main */   
	
	

/******************************************************************************
 Purpose: Finds a root of scalar, non-linear function f using the integer 
 bisection  method. a and b make up the initial bracket to start bisecting from.
 Only implement this after you have the floating point function implemented
 
 Where: ifunc1arg f - INTEGER function whose root is to be determined
					  must take a single argument of type long and return
		long a	  - initial Qn root bracket guess
		long b	  - initial Qn root bracket guess
		long atol   - absolute Qn error termination tolerance,
		int verb	- verbose flag, 1 = TRUE, 0 = FALSE
		
Returns: long - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN				 
******************************************************************************/
long ibisection(ifunc1arg f, long a, long b, long atol, int verb) {
	long x1 = a;
	long x2 = b;
	long x3;
	long val3;
	long err;
	int iter = 0;

	long val1 = (*f)(x1);

	if(abs(val1) <= atol) {
		printf("here");
		return x1;
	}

	double val2 = (*f)(x2);

	if(abs(val2) <= atol) {
		printf("here");
		return x2;
	}

	if (SIGN(val1) == SIGN(val2)) {
		fprintf(stderr, "No root inbetween backets");
		return iNAN;
	}

	err = x2 - x1;
	//int n = ceil((log(FIX_TO_FLOAT(x2-x1))/FIX_TO_FLOAT(atol))/log(2.0));
	int n = 1000;
	for(int i = 0; i<n; i++) {
		x3 = Qn_DIVIDE(x1+x2, FIX_2);
		val3 = (*f)(x3);

		if(verb) {
			printf("iter:%d\ta:%f\tb:%f\tx:%f\terr:%f\n", iter, FIX_TO_FLOAT(x1), 
					FIX_TO_FLOAT(x2), FIX_TO_FLOAT(x3), FIX_TO_FLOAT(err));
		}

		if(abs(val3) <= atol) {
			return x3;
		}

		if(SIGN(val1) != SIGN(val3)) {
			err = abs(x3-x2);
			x2 = x3;
			val2 = val3;
		}
		else {
			err =abs(x3-x1);
			x1 = x3;
			val1 = val3;
		}
		iter++;
	}
	printf("n:%d\n", n);
	return (long) Qn_DIVIDE(x1+x2, FIX_2);
}	
	
/*---------------------------------------------------------------------------
  This function implements the real line equation to be solved.  
  
  Where:   double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double rline(double x){
	return(0.5*x-0.3);
}


/*---------------------------------------------------------------------------
  This function implements the integer line equation to be solved.  
  
  Where:	long x - the value to evaluate
  Returns: long - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
long iline(long x){
	/* Implement return(0.5*x-.3) as Qn code */
	return (MUL_FIX(FLOAT_TO_FIX(0.5), x) - FLOAT_TO_FIX(0.3)); 
}

